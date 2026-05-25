from __future__ import annotations

import uuid
from typing import Any

import pymysql
import pytest
from pymysql.connections import Connection
from pymysql.cursors import DictCursor

VALID_START = "2025-08-12"
VALID_END = "2025-12-05"


def insert_course(
    db: Connection[DictCursor],
    course_id: str,
    term_id: str,
    title: str | None = None,
    description: str = "Operating Systems",
    start: str = VALID_START,
    end: str = VALID_END,
    num_credits: int = 3,
    active: bool = False,
) -> str:
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO courses (id, term_id, title, description, "
            "start_date, end_date, num_credits, active) VALUES (%s, %s, %s, "
            "%s, %s, %s, %s, %s)",
            (
                course_id,
                term_id,
                title or "CMPE 142",
                description,
                start,
                end,
                num_credits,
                active,
            ),
        )
    return course_id


def find_course(
    db: Connection[DictCursor], course_id: str
) -> dict[str, Any] | None:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, term_id, title, description, start_date, end_date, "
            "num_credits, active FROM courses WHERE id = %s",
            (course_id,),
        )
        return cursor.fetchone()


def find_all_courses(db: Connection[DictCursor]) -> tuple[dict[str, Any], ...]:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, term_id, title, description, start_date, end_date, "
            "num_credits, active FROM courses"
        )
        return cursor.fetchall()


def find_courses_by_term(
    db: Connection[DictCursor], term_id: str
) -> tuple[dict[str, Any], ...]:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, term_id, title, description, start_date, end_date, "
            "num_credits, active FROM courses WHERE term_id = %s",
            (term_id,),
        )
        return cursor.fetchall()


# ====================================
# SMOKE TESTS
# ====================================


def test_insert_and_find_by_id(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    result = find_course(db, course_id)

    assert result is not None
    assert result["id"] == course_id
    assert result["title"] == "CMPE 142"
    assert result["description"] == "Operating Systems"
    assert str(result["start_date"]) == VALID_START
    assert str(result["end_date"]) == VALID_END
    assert result["num_credits"] == 3
    assert result["active"] == 0


def test_insert_and_find_all(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], title="CMPE 142")
    insert_course(
        db,
        str(uuid.uuid4()),
        parent_term["id"],
        title="ENGR 195A",
        num_credits=1,
        active=True,
    )

    results = find_all_courses(db)
    assert len(results) >= 2


def test_insert_and_find_by_parent_id(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], title="CMPE 142")
    insert_course(db, str(uuid.uuid4()), parent_term["id"], title="ENGR 195A")

    results = find_courses_by_term(db, parent_term["id"])
    assert len(results) == 2


def test_insert_active_true(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], active=True)
    result = find_course(db, course_id)

    assert result is not None
    assert result["active"] == 1


def test_insert_active_false(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], active=False)
    result = find_course(db, course_id)

    assert result is not None
    assert result["active"] == 0


def test_insert_empty_description(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], description="")
    result = find_course(db, course_id)

    assert result is not None
    assert result["description"] == "" or result["description"] is None


def test_update_title(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE courses SET title = %s WHERE id = %s",
            ("CMPE 180", course_id),
        )

    result = find_course(db, course_id)
    assert result is not None
    assert result["title"] == "CMPE 180"


def test_update_description(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE courses SET description = %s WHERE id = %s",
            ("Updated description.", course_id),
        )

    result = find_course(db, course_id)
    assert result is not None
    assert result["description"] == "Updated description."


def test_update_num_credits(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE courses SET num_credits = %s WHERE id = %s", (4, course_id)
        )

    result = find_course(db, course_id)
    assert result is not None
    assert result["num_credits"] == 4


def test_update_active(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], active=False)
    with db.cursor() as cursor:
        cursor.execute(
            "UPDATE courses SET active = %s WHERE id = %s", (True, course_id)
        )

    result = find_course(db, course_id)
    assert result is not None
    assert result["active"] == 1


def test_remove_course(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM courses WHERE id = %s", (course_id,))

    assert find_course(db, course_id) is None


def test_find_by_id_not_found(db: Connection[DictCursor]) -> None:
    assert find_course(db, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee") is None


def test_find_all_empty(db: Connection[DictCursor]) -> None:
    assert len(find_all_courses(db)) == 0


def test_find_by_parent_id_empty(
    db: Connection[DictCursor], parent_term: dict[str, Any]
) -> None:
    assert len(find_courses_by_term(db, parent_term["id"])) == 0


# ====================================
# EDGE CASES
# ====================================


def test_insert_duplicate_title_under_same_term_raises(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], title="CMPE 142")

    with pytest.raises(pymysql.IntegrityError):
        insert_course(
            db, str(uuid.uuid4()), parent_term["id"], title="CMPE 142"
        )


def test_insert_invalid_term_id_raises(
    db: Connection[DictCursor], course_id: str
) -> None:
    with pytest.raises(pymysql.IntegrityError):
        insert_course(db, course_id, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee")


def test_find_by_parent_id_only_returns_courses_belonging_to_term(
    db: Connection[DictCursor],
    course_id: str,
    parent_term: dict[str, Any],
    term_id: str,
) -> None:
    other_term_id = str(uuid.uuid4())
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO terms (id, title, start_date, end_date, active) "
            "VALUES (%s, %s, %s, %s, %s)",
            (other_term_id, "Spring 2026", "2026-01-20", "2026-05-23", True),
        )
    insert_course(db, str(uuid.uuid4()), other_term_id, title="CMPE 180")
    insert_course(db, course_id, parent_term["id"], title="CMPE 142")

    results = find_courses_by_term(db, parent_term["id"])
    assert len(results) == 1
    assert results[0]["title"] == "CMPE 142"


def test_insert_round_trips_id(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    result = find_course(db, course_id)

    assert result is not None
    assert result["id"] == course_id


def test_insert_zero_credits(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"], num_credits=0)
    result = find_course(db, course_id)

    assert result is not None
    assert result["num_credits"] == 0


def test_insert_year_boundary_dates(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(
        db, course_id, parent_term["id"], start="2024-12-31", end="2025-01-01"
    )
    result = find_course(db, course_id)

    assert result is not None
    assert str(result["start_date"]) == "2024-12-31"
    assert str(result["end_date"]) == "2025-01-01"


def test_insert_leap_year_start_date(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(
        db, course_id, parent_term["id"], start="2024-02-29", end="2024-06-01"
    )
    result = find_course(db, course_id)

    assert result is not None
    assert str(result["start_date"]) == "2024-02-29"


def test_remove_nonexistent_id_no_raise(db: Connection[DictCursor]) -> None:
    with db.cursor() as cursor:
        cursor.execute(
            "DELETE FROM courses WHERE id = %s",
            ("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",),
        )


def test_cascade_delete_removes_course_with_term(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, Any]
) -> None:
    insert_course(db, course_id, parent_term["id"])
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM terms WHERE id = %s", (parent_term["id"],))

    assert find_course(db, course_id) is None
