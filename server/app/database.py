from collections.abc import Generator
import os
from collections.abc import Generator

import pymysql
from dotenv import load_dotenv
from pymysql.connections import Connection
from pymysql.cursors import DictCursor

load_dotenv()


def get_db() -> Generator[Connection[DictCursor], None, None]:
    db_user = os.getenv("DB_USER")
    db_password = os.getenv("DB_PASSWORD")
    db_name = os.getenv("DB_NAME")

    if not db_user:
        raise ValueError("DB_USER not set")

    if not db_password:
        raise ValueError("DB_PASSWORD not set")

    if not db_name:
        raise ValueError("DB_NAME not set")

    conn: Connection[DictCursor] = pymysql.connect(
        host=os.getenv("DB_HOST") or "localhost",
        port=int(os.getenv("DB_PORT") or 3306),
        user=db_user,
        password=db_password,
        database=db_name,
        cursorclass=DictCursor,
    )

    try:
        yield conn
    finally:
        conn.close()