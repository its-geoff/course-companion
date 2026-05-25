from __future__ import annotations

import uuid
import pytest
import pymysql
from pymysql.connections import Connection
from pymysql.cursors import DictCursor
from typing import Any


VALID_START = "2025-08-12"
VALID_END = "2025-12-05"


def insert_term(
    db: Connection[DictCursor],
    term_id: str,
    title: str | None = None,
    start: str = VALID_START,
    end: str = VALID_END,
    active: bool = False,
) -> str:
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO terms (id, title, start_date, end_date, active) VALUES (%s, %s, %s, %s, %s)",
            (term_id, title or "Fall 2025", start, end, active),
        )
    return term_id


def find_term(db: Connection[DictCursor], term_id: str) -> dict[str, Any] | None:
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, title, start_date, end_date, active FROM terms WHERE id = %s",
            (term_id,),
        )
        return cursor.fetchone()


def find_all_terms(db: Connection[DictCursor]) -> tuple[dict[str, Any], ...]:
    with db.cursor() as cursor:
        cursor.execute("SELECT id, title, start_date, end_date, active FROM terms")
        return cursor.fetchall()


# ====================================
# SMOKE TESTS
# ====================================

def test_insert_and_find_by_id(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id)
    result = find_term(db, term_id)

    assert result is not None
    assert result["id"] == term_id
    assert result["title"] == "Fall 2025"
    assert str(result["start_date"]) == VALID_START
    assert str(result["end_date"]) == VALID_END
    assert result["active"] == 0


def test_insert_and_find_all(db: Connection[DictCursor], term_id: str) -> None:
    id2 = str(uuid.uuid4())
    insert_term(db, term_id, title="Fall 2025")
    insert_term(db, id2, title="Spring 2026", start="2026-01-20", end="2026-05-23", active=True)

    results = find_all_terms(db)
    assert len(results) >= 2


def test_insert_active_true(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, active=True)
    result = find_term(db, term_id)

    assert result is not None
    assert result["active"] == 1


def test_insert_active_false(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, active=False)
    result = find_term(db, term_id)

    assert result is not None
    assert result["active"] == 0


def test_update_title(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id)
    with db.cursor() as cursor:
        cursor.execute("UPDATE terms SET title = %s WHERE id = %s", ("Spring 2026", term_id))

    result = find_term(db, term_id)
    assert result is not None
    assert result["title"] == "Spring 2026"


def test_update_start_date(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id)
    with db.cursor() as cursor:
        cursor.execute("UPDATE terms SET start_date = %s WHERE id = %s", ("2025-09-01", term_id))

    result = find_term(db, term_id)
    assert result is not None
    assert str(result["start_date"]) == "2025-09-01"


def test_update_end_date(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id)
    with db.cursor() as cursor:
        cursor.execute("UPDATE terms SET end_date = %s WHERE id = %s", ("2025-12-20", term_id))

    result = find_term(db, term_id)
    assert result is not None
    assert str(result["end_date"]) == "2025-12-20"


def test_update_active(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, active=False)
    with db.cursor() as cursor:
        cursor.execute("UPDATE terms SET active = %s WHERE id = %s", (True, term_id))

    result = find_term(db, term_id)
    assert result is not None
    assert result["active"] == 1


def test_remove_term(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id)
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM terms WHERE id = %s", (term_id,))

    assert find_term(db, term_id) is None


def test_find_by_id_not_found(db: Connection[DictCursor]) -> None:
    result = find_term(db, "aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee")
    assert result is None


def test_find_all_empty(db: Connection[DictCursor]) -> None:
    results = find_all_terms(db)
    assert len(results) == 0


# ====================================
# EDGE CASES
# ====================================

def test_insert_duplicate_title_raises(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, title="Fall 2025")

    with pytest.raises(pymysql.IntegrityError):
        insert_term(db, str(uuid.uuid4()), title="Fall 2025")


def test_insert_round_trips_id(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id)
    result = find_term(db, term_id)

    assert result is not None
    assert result["id"] == term_id


def test_insert_year_boundary_dates(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, start="2024-12-31", end="2025-01-01")
    result = find_term(db, term_id)

    assert result is not None
    assert str(result["start_date"]) == "2024-12-31"
    assert str(result["end_date"]) == "2025-01-01"


def test_insert_leap_year_date(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, start="2024-02-29", end="2024-06-01")
    result = find_term(db, term_id)

    assert result is not None
    assert str(result["start_date"]) == "2024-02-29"


def test_insert_same_day_start_and_end(db: Connection[DictCursor], term_id: str) -> None:
    insert_term(db, term_id, start="2025-06-15", end="2025-06-15")
    result = find_term(db, term_id)

    assert result is not None
    assert result["start_date"] == result["end_date"]


def test_remove_nonexistent_id_no_raise(db: Connection[DictCursor]) -> None:
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM terms WHERE id = %s", ("aaaaaaaa-bbbb-cccc-dddd-eeeeeeeeeeee",))


def test_find_all_returns_two_inserted_terms(db: Connection[DictCursor], term_id: str) -> None:
    id2 = str(uuid.uuid4())
    insert_term(db, term_id, title="Fall 2025")
    insert_term(db, id2, title="Spring 2026", start="2026-01-20", end="2026-05-23", active=True)

    results = find_all_terms(db)
    assert len(results) == 2