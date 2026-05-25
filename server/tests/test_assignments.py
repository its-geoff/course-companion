from __future__ import annotations

import uuid
from typing import Any

import pymysql
import pytest
from pymysql.connections import Connection
from pymysql.cursors import DictCursor

VALID_DUE = "2025-11-20"


def insert_assignment(
    db: Connection[DictCursor],
    assignment_id: str,
    course_id: str,
    title: str | None = None,
    description: str = "Focus on variables and strings.",
    category: str = "Homework",
    due_date: str = VALID_DUE,
    completed: bool = True,
    grade: float = 95.18,
) -> str:
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO assignments (id, course_id, title, description, "
            "category, due_date, completed, grade) VALUES (%s, %s, %s, %s, "
            "%s, %s, %s, %s)",
            (
                assignment_id,
                course_id,
                title or "Homework 3",
                description,
                category,
                due_date,
                completed,
                grade,
            ),
        )
    return assignment_id


def find_assignment(
    db: Connection[DictCursor], assignment_id: str
) -> dict[str, Any] | None:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, course_id, title, description, category, due_date, "
            "completed, grade FROM assignments WHERE id = %s",
            (assignment_id,),
        )
        return cursor.fetchone()


def find_all_assignments(
    db: Connection[DictCursor],
) -> tuple[dict[str, Any], ...]:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, course_id, title, description, category, due_date, "
            "completed, grade FROM assignments"
        )
        return cursor.fetchall()


def find_assignments_by_course(
    db: Connection[DictCursor], course_id: str
) -> tuple[dict[str, Any], ...]:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, course_id, title, description, category, due_date, "
            "completed, grade FROM assignments WHERE course_id = %s",
            (course_id,),
        )
        return cursor.fetchall()


# ====================================
# SMOKE TESTS
# ====================================


def test_insert_and_find_by_id(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert result["id"] == assignment_id
    assert result["title"] == "Homework 3"
    assert result["description"] == "Focus on variables and strings."
    assert result["category"] == "Homework"
    assert str(result["due_date"]) == VALID_DUE
    assert result["completed"] == 1
    assert abs(result["grade"] - 95.18) < 0.01


def test_insert_and_find_all(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], title="Homework 3"
    )
    insert_assignment(
        db,
        str(uuid.uuid4()),
        parent_course["id"],
        title="Midterm",
        category="Midterm",
        due_date="2025-10-15",
        grade=88.0,
    )

    results = find_all_assignments(db)
    assert len(results) >= 2


def test_insert_and_find_by_parent_id(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], title="Homework 3"
    )
    insert_assignment(
        db,
        str(uuid.uuid4()),
        parent_course["id"],
        title="Midterm",
        category="Midterm",
        due_date="2025-10-15",
        grade=88.0,
    )

    results = find_assignments_by_course(db, parent_course["id"])
    assert len(results) == 2


def test_insert_completed_true(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"], completed=True)
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert result["completed"] == 1


def test_insert_completed_false(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], completed=False, grade=0.0
    )
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert result["completed"] == 0


def test_insert_empty_description(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"], description="")
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert result["description"] == "" or result["description"] is None


def test_update_title(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE assignments SET title = %s WHERE id = %s",
            ("Homework 4", assignment_id),
        )

    result = find_assignment(db, assignment_id)
    assert result is not None
    assert result["title"] == "Homework 4"


def test_update_description(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE assignments SET description = %s WHERE id = %s",
            ("Updated description.", assignment_id),
        )

    result = find_assignment(db, assignment_id)
    assert result is not None
    assert result["description"] == "Updated description."


def test_update_grade(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"], grade=80.0)
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE assignments SET grade = %s WHERE id = %s",
            (92.5, assignment_id),
        )

    result = find_assignment(db, assignment_id)
    assert result is not None
    assert abs(result["grade"] - 92.5) < 0.01


def test_update_completed(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], completed=False, grade=0.0
    )
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE assignments SET completed = %s, grade = %s WHERE id = %s",
            (True, 88.0, assignment_id),
        )

    result = find_assignment(db, assignment_id)
    assert result is not None
    assert result["completed"] == 1
    assert abs(result["grade"] - 88.0) < 0.01


def test_update_due_date(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE assignments SET due_date = %s WHERE id = %s",
            ("2025-12-01", assignment_id),
        )

    result = find_assignment(db, assignment_id)
    assert result is not None
    assert str(result["due_date"]) == "2025-12-01"


def test_remove_assignment(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "DELETE FROM assignments WHERE id = %s", (assignment_id,)
        )

    assert find_assignment(db, assignment_id) is None


def test_find_by_id_not_found(db: Connection[DictCursor]) -> None:
    assert find_assignment(db, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee") is None


def test_find_all_empty(db: Connection[DictCursor]) -> None:
    assert len(find_all_assignments(db)) == 0


def test_find_by_parent_id_empty(
    db: Connection[DictCursor], parent_course: dict[str, Any]
) -> None:
    assert len(find_assignments_by_course(db, parent_course["id"])) == 0


# ====================================
# EDGE CASES
# ====================================


def test_insert_duplicate_title_under_same_course_raises(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], title="Homework 3"
    )

    with pytest.raises(pymysql.IntegrityError):
        insert_assignment(
            db, str(uuid.uuid4()), parent_course["id"], title="Homework 3"
        )


def test_insert_invalid_course_id_raises(
    db: Connection[DictCursor], assignment_id: str
) -> None:
    with pytest.raises(pymysql.IntegrityError):
        insert_assignment(
            db, assignment_id, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee"
        )


def test_find_by_parent_id_only_returns_assignments_belonging_to_course(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
    parent_term: dict[str, Any],
) -> None:
    other_course_id = str(uuid.uuid4())
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO courses (id, term_id, title, description, "
            "start_date, end_date, num_credits, active) VALUES (%s, %s, %s, "
            "%s, %s, %s, %s, %s)",
            (
                other_course_id,
                parent_term["id"],
                "ENGR 195A",
                "",
                "2025-08-14",
                "2025-12-18",
                1,
                True,
            ),
        )
    insert_assignment(
        db,
        str(uuid.uuid4()),
        other_course_id,
        title="Final Report",
        category="Final Exam",
        due_date="2025-12-10",
        completed=False,
        grade=0.0,
    )
    insert_assignment(
        db, assignment_id, parent_course["id"], title="Homework 3"
    )

    results = find_assignments_by_course(db, parent_course["id"])
    assert len(results) == 1
    assert results[0]["title"] == "Homework 3"


def test_insert_round_trips_id(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert result["id"] == assignment_id


def test_insert_grade_boundary_low(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], completed=True, grade=0.0
    )
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert abs(result["grade"] - 0.0) < 0.01


def test_insert_grade_boundary_high(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], completed=True, grade=150.0
    )
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert abs(result["grade"] - 150.0) < 0.01


def test_insert_leap_year_due_date(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(
        db, assignment_id, parent_course["id"], due_date="2024-02-29"
    )
    result = find_assignment(db, assignment_id)

    assert result is not None
    assert str(result["due_date"]) == "2024-02-29"


def test_remove_nonexistent_id_no_raise(db: Connection[DictCursor]) -> None:
    with db.cursor() as cursor:
        cursor.execute(
            "DELETE FROM assignments WHERE id = %s",
            ("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",),
        )


def test_cascade_delete_removes_assignment_with_course(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "DELETE FROM courses WHERE id = %s", (parent_course["id"],)
        )

    assert find_assignment(db, assignment_id) is None


def test_cascade_delete_removes_assignment_with_term(
    db: Connection[DictCursor],
    assignment_id: str,
    parent_course: dict[str, Any],
    parent_term: dict[str, Any],
) -> None:
    insert_assignment(db, assignment_id, parent_course["id"])
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM terms WHERE id = %s", (parent_term["id"],))

    assert find_assignment(db, assignment_id) is None
