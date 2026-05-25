from __future__ import annotations

import os
import uuid
from pathlib import Path
from typing import Generator

import pymysql
import pymysql.cursors
import pytest
from dotenv import load_dotenv
from pymysql.connections import Connection
from pymysql.cursors import DictCursor

load_dotenv(Path(__file__).parent.parent / ".env")


@pytest.fixture(scope="session")
def db_connection() -> Generator[Connection[DictCursor], None, None]:
    conn = pymysql.connect(
        host=os.getenv("DB_HOST") or "localhost",
        port=int(os.getenv("DB_PORT") or 3306),
        user=os.getenv("DB_USER")
        or (_ for _ in ()).throw(ValueError("DB_USER not set")),
        password=os.getenv("DB_PASSWORD")
        or (_ for _ in ()).throw(ValueError("DB_PASSWORD not set")),
        database=os.getenv("DB_NAME")
        or (_ for _ in ()).throw(ValueError("DB_NAME not set")),
        cursorclass=pymysql.cursors.DictCursor,
    )
    yield conn
    conn.close()


@pytest.fixture
def db(
    db_connection: Connection[DictCursor],
) -> Generator[Connection[DictCursor], None, None]:
    db_connection.begin()
    yield db_connection
    db_connection.rollback()


@pytest.fixture
def term_id() -> str:
    return str(uuid.uuid4())


@pytest.fixture
def course_id() -> str:
    return str(uuid.uuid4())


@pytest.fixture
def assignment_id() -> str:
    return str(uuid.uuid4())


@pytest.fixture
def parent_term(db: Connection[DictCursor], term_id: str) -> dict[str, object]:
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO terms (id, title, start_date, end_date, active) "
            "VALUES (%s, %s, %s, %s, %s)",
            (term_id, "Fall 2025", "2025-08-12", "2025-12-05", False),
        )
    return {
        "id": term_id,
        "title": "Fall 2025",
        "start_date": "2025-08-12",
        "end_date": "2025-12-05",
        "active": False,
    }


@pytest.fixture
def parent_course(
    db: Connection[DictCursor], course_id: str, parent_term: dict[str, object]
) -> dict[str, object]:
    with db.cursor() as cursor:
        cursor.execute(
            "INSERT INTO courses (id, term_id, title, description, "
            "start_date, end_date, num_credits, active) VALUES (%s, %s, %s, "
            "%s, %s, %s, %s, %s)",
            (
                course_id,
                parent_term["id"],
                "CMPE 142",
                "Operating Systems",
                "2025-08-12",
                "2025-12-05",
                3,
                False,
            ),
        )
    return {
        "id": course_id,
        "term_id": parent_term["id"],
        "title": "CMPE 142",
        "description": "Operating Systems",
        "start_date": "2025-08-12",
        "end_date": "2025-12-05",
        "num_credits": 3,
        "active": False,
    }
