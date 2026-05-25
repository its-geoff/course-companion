from datetime import date
from typing import Optional

import pymysql
from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from pymysql.connections import Connection

from app.database import get_db

router = APIRouter()


class CourseCreate(BaseModel):
    id: str
    term_id: str
    title: str
    description: Optional[str] = None
    start_date: date
    end_date: date
    num_credits: int = 3
    active: bool = True


class CourseResponse(CourseCreate):
    pass


@router.get("/", response_model=list[CourseResponse])
def get_courses(db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, term_id, title, description, start_date, end_date, "
            "num_credits, active FROM courses"
        )
        return cursor.fetchall()


@router.get("/by-term/{term_id}", response_model=list[CourseResponse])
def get_courses_by_term(term_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, term_id, title, description, start_date, end_date, "
            "num_credits, active FROM courses WHERE term_id = %s",
            (term_id,),
        )
        return cursor.fetchall()


@router.get("/{course_id}", response_model=CourseResponse)
def get_course(course_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, term_id, title, description, start_date, end_date, "
            "num_credits, active FROM courses WHERE id = %s",
            (course_id,),
        )
        course = cursor.fetchone()
    if not course:
        raise HTTPException(status_code=404, detail="Course not found")
    return course


@router.post("/", response_model=CourseResponse, status_code=201)
def create_course(course: CourseCreate, db: Connection = Depends(get_db)):
    try:
        with db.cursor() as cursor:
            cursor.execute(
                "INSERT INTO courses (id, term_id, title, description, "
                "start_date, end_date, num_credits, active) VALUES (%s, %s, "
                "%s, %s, %s, %s, %s, %s)",
                (
                    course.id,
                    course.term_id,
                    course.title,
                    course.description,
                    course.start_date,
                    course.end_date,
                    course.num_credits,
                    course.active,
                ),
            )
        db.commit()
    except pymysql.IntegrityError:
        raise HTTPException(
            status_code=409,
            detail="A course with this title already exists in this term",
        )
    return course


@router.put("/{course_id}", response_model=CourseResponse)
def update_course(
    course_id: str, course: CourseCreate, db: Connection = Depends(get_db)
):
    try:
        with db.cursor() as cursor:
            cursor.execute(
                "UPDATE courses SET title = %s, description = %s, "
                "start_date = %s, end_date = %s, num_credits = %s, "
                "active = %s WHERE id = %s",
                (
                    course.title,
                    course.description,
                    course.start_date,
                    course.end_date,
                    course.num_credits,
                    course.active,
                    course_id,
                ),
            )
        db.commit()
    except pymysql.IntegrityError:
        raise HTTPException(
            status_code=409,
            detail="A course with this title already exists in this term",
        )
    if cursor.rowcount == 0:
        raise HTTPException(status_code=404, detail="Course not found")
    return course


@router.delete("/{course_id}", status_code=204)
def delete_course(course_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM courses WHERE id = %s", (course_id,))
    db.commit()
    if cursor.rowcount == 0:
        raise HTTPException(status_code=404, detail="Course not found")
