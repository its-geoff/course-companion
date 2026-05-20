from fastapi import APIRouter, Depends, HTTPException
from pydantic import BaseModel
from datetime import date
from typing import Optional
import pymysql
from app.database import get_db
from pymysql.connections import Connection

router = APIRouter()

class AssignmentCreate(BaseModel):
    id: str
    course_id: str
    title: str
    description: Optional[str] = None
    category: str
    due_date: date
    completed: bool = False
    grade: float = 0.0

class AssignmentResponse(AssignmentCreate):
    pass

@router.get("/", response_model=list[AssignmentResponse])
def get_assignments(db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, course_id, title, description, category, due_date, completed, grade FROM assignments"
        )
        return cursor.fetchall()

@router.get("/by-course/{course_id}", response_model=list[AssignmentResponse])
def get_assignments_by_course(course_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, course_id, title, description, category, due_date, completed, grade FROM assignments WHERE course_id = %s",
            (course_id,)
        )
        return cursor.fetchall()

@router.get("/{assignment_id}", response_model=AssignmentResponse)
def get_assignment(assignment_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute(
            "SELECT id, course_id, title, description, category, due_date, completed, grade FROM assignments WHERE id = %s",
            (assignment_id,)
        )
        assignment = cursor.fetchone()
    if not assignment:
        raise HTTPException(status_code=404, detail="Assignment not found")
    return assignment

@router.post("/", response_model=AssignmentResponse, status_code=201)
def create_assignment(assignment: AssignmentCreate, db: Connection = Depends(get_db)):
    try:
        with db.cursor() as cursor:
            cursor.execute(
                "INSERT INTO assignments (id, course_id, title, description, category, due_date, completed, grade) VALUES (%s, %s, %s, %s, %s, %s, %s, %s)",
                (assignment.id, assignment.course_id, assignment.title, assignment.description, assignment.category, assignment.due_date, assignment.completed, assignment.grade)
            )
        db.commit()
    except pymysql.IntegrityError:
        raise HTTPException(status_code=409, detail="An assignment with this title already exists in this course")
    return assignment

@router.put("/{assignment_id}", response_model=AssignmentResponse)
def update_assignment(assignment_id: str, assignment: AssignmentCreate, db: Connection = Depends(get_db)):
    try:
        with db.cursor() as cursor:
            cursor.execute(
                "UPDATE assignments SET title = %s, description = %s, category = %s, due_date = %s, completed = %s, grade = %s WHERE id = %s",
                (assignment.title, assignment.description, assignment.category, assignment.due_date, assignment.completed, assignment.grade, assignment_id)
            )
        db.commit()
    except pymysql.IntegrityError:
        raise HTTPException(status_code=409, detail="An assignment with this title already exists in this course")
    if cursor.rowcount == 0:
        raise HTTPException(status_code=404, detail="Assignment not found")
    return assignment

@router.delete("/{assignment_id}", status_code=204)
def delete_assignment(assignment_id: str, db: Connection = Depends(get_db)):
    with db.cursor() as cursor:
        cursor.execute("DELETE FROM assignments WHERE id = %s", (assignment_id,))
    db.commit()
    if cursor.rowcount == 0:
        raise HTTPException(status_code=404, detail="Assignment not found")