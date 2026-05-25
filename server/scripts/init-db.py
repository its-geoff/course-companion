import os
from pathlib import Path

import pymysql
from dotenv import load_dotenv

load_dotenv(Path(__file__).parent.parent / ".env")

conn = pymysql.connect(
    host=os.getenv("DB_HOST", "localhost"),
    port=int(os.getenv("DB_PORT", "3306")),
    user=os.getenv("DB_USER"),
    password=os.getenv("DB_PASSWORD", ""),
)

schemas = [
    os.getenv("DB_NAME", "course_companion"),
    os.getenv("TEST_DB_SCHEMA", "course_companion_test"),
]


def init_schema(cursor: pymysql.cursors.Cursor, schema: str) -> None:
    cursor.execute(f"CREATE DATABASE IF NOT EXISTS `{schema}`")
    cursor.execute(f"USE `{schema}`")
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS terms (
            id          CHAR(36)        NOT NULL PRIMARY KEY,
            title       VARCHAR(255)    NOT NULL UNIQUE,
            start_date  DATE            NOT NULL,
            end_date    DATE            NOT NULL,
            active      BOOLEAN         NOT NULL DEFAULT TRUE
        )
    """)
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS courses (
            id            CHAR(36)        NOT NULL PRIMARY KEY,
            term_id       CHAR(36)        NOT NULL,
            title         VARCHAR(255)    NOT NULL,
            description   TEXT,
            start_date    DATE            NOT NULL,
            end_date      DATE            NOT NULL,
            num_credits   INT             NOT NULL DEFAULT 3,
            active        BOOLEAN         NOT NULL DEFAULT TRUE,
            UNIQUE (term_id, title),
            FOREIGN KEY (term_id) REFERENCES terms(id) ON DELETE CASCADE
        )
    """)
    cursor.execute("""
        CREATE TABLE IF NOT EXISTS assignments (
            id          CHAR(36)        NOT NULL PRIMARY KEY,
            course_id   CHAR(36)        NOT NULL,
            title       VARCHAR(255)    NOT NULL,
            description TEXT,
            category    VARCHAR(100)    NOT NULL,
            due_date    DATE            NOT NULL,
            completed   BOOLEAN         NOT NULL DEFAULT FALSE,
            grade       FLOAT           NOT NULL DEFAULT 0.0,
            UNIQUE (course_id, title),
            FOREIGN KEY (course_id) REFERENCES courses(id) ON DELETE CASCADE
        )
    """)


with conn.cursor() as cursor:
    for schema in schemas:
        init_schema(cursor, schema)
        print(f"Database '{schema}' initialized.")

conn.commit()
conn.close()
