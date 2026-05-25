import sqlite3
from sqlite3 import Error
from getpass import getpass 

DB_FILE = "student_db.db"
TEACHER_USER = "teacher"
TEACHER_PASS = "admin123"
conn = None

def create_connection():
    """Create a database connection to the SQLite database specified by DB_FILE"""
    global conn
    try:
        conn = sqlite3.connect(DB_FILE)
        return True
    except Error as e:
        print(f"Error connecting to DB: {e}")
        return False

def setup_database():
    """Create the students table if it doesn't exist."""
    if conn is None:
        print("Database connection not established.")
        return False
        
    sql = """
        CREATE TABLE IF NOT EXISTS students (
            id INTEGER PRIMARY KEY,
            name TEXT NOT NULL,
            father TEXT,
            mother TEXT,
            address TEXT,
            email TEXT,
            mark1 REAL,
            mark2 REAL,
            mark3 REAL,
            mark4 REAL,
            mark5 REAL,
            total REAL,
            percentage REAL,
            fees REAL,
            password TEXT NOT NULL
        );
    """
    try:
        conn.execute(sql)
        conn.commit()
        return True
    except Error as e:
        print(f"Error creating table: {e}")
        return False

def add_student():
    """Adds a new student record to the database."""
    print("\n--- Add New Student ---")
    try:
        student_id = int(input("Enter Student ID: "))
    except ValueError:
        print("Invalid input for ID. Returning.")
        return
        
    name = input("Name: ")
    father = input("Father's Name: ")
    mother = input("Mother's Name: ")
    address = input("Address: ")
    email = input("Email: ")
    
    marks = []
    total = 0.0
    for i in range(5):
        try:
            mark = float(input(f"Marks for subject {i+1}: "))
            marks.append(mark)
            total += mark
        except ValueError:
            print("Invalid input for marks. Returning.")
            return

    percentage = total / 5.0
    
    try:
        fees = float(input("Fees: "))
    except ValueError:
        print("Invalid input for fees. Returning.")
        return
        
    password = getpass("Set Password for student: ")

    sql = """
        INSERT INTO students 
        (id, name, father, mother, address, email, mark1, mark2, mark3, mark4, mark5, total, percentage, fees, password) 
        VALUES (?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?, ?);
    """
    params = (student_id, name, father, mother, address, email, *marks, total, percentage, fees, password)
    
    try:
        conn.execute(sql, params)
        conn.commit()
        print(f"\nStudent added successfully. Total: {total:.2f}, Percentage: {percentage:.2f}%.")
    except Error as e:
        print(f"Error inserting student: {e}")

def view_all_students():
    """Displays a summary table of all students."""
    sql = "SELECT id, name, total, percentage, fees FROM students ORDER BY id;"
    cursor = conn.cursor()
    cursor.execute(sql)
    students = cursor.fetchall()

    print("\n--- All Students ---")
    if not students:
        print("No students found.")
        return
        
    print(f"{'ID':<8}{'Name':<25}{'Total':<10}{'Percentage':<12}{'Fees':<10}")
    print("-" * 65)

    for row in students:
        s_id, name, total, percentage, fees = row
        print(f"{s_id:<8}{name:<25}{total:<10.2f}{percentage:<12.2f}{fees:<10.2f}")

def view_student_by_id(student_id):
    """Displays detailed information for a specific student ID."""
    sql = "SELECT * FROM students WHERE id=?;"
    cursor = conn.cursor()
    cursor.execute(sql, (student_id,))
    student = cursor.fetchone()

    if student:
        s_id, name, father, mother, address, email, m1, m2, m3, m4, m5, total, percentage, fees, _ = student
        
        print("\n--- Student Details ---")
        print(f"ID: {s_id}")
        print(f"Name: {name}")
        print(f"Father: {father}")
        print(f"Mother: {mother}")
        print(f"Address: {address}")
        print(f"Email: {email}")
        print(f"Marks: S1: {m1:.2f}, S2: {m2:.2f}, S3: {m3:.2f}, S4: {m4:.2f}, S5: {m5:.2f}")
        print(f"Total: {total:.2f} | Percentage: {percentage:.2f}% | Fees: {fees:.2f}")
    else:
        print(f"No student found with ID {student_id}.")

def remove_student():
    """Removes a student record by ID."""
    try:
        student_id = int(input("Enter Student ID to remove: "))
    except ValueError:
        print("Invalid input for ID. Returning.")
        return
        
    sql = "DELETE FROM students WHERE id=?;"
    cursor = conn.cursor()
    cursor.execute(sql, (student_id,))
    conn.commit()

    if cursor.rowcount > 0:
        print(f"Student with ID {student_id} successfully removed.")
    else:
        print(f"No student found with ID {student_id} to remove.")

def update_student():
    """Updates marks and/or fees for a student."""
    try:
        student_id = int(input("Enter Student ID to update: "))
    except ValueError:
        print("Invalid input for ID. Returning.")
        return
    
    cursor = conn.cursor()
    cursor.execute("SELECT id FROM students WHERE id=?", (student_id,))
    if cursor.fetchone() is None:
        print(f"Error: No student found with ID {student_id}.")
        return

    choice = input("Update Marks (Y/N)? ").strip().lower()
    if choice == 'y':
        marks = []
        total = 0.0
        try:
            for i in range(5):
                mark = float(input(f"New Marks for subject {i+1}: "))
                marks.append(mark)
                total += mark
            percentage = total / 5.0
            
            mark_update_sql = "UPDATE students SET mark1=?,mark2=?,mark3=?,mark4=?,mark5=?,total=?,percentage=? WHERE id=?"
            cursor.execute(mark_update_sql, (*marks, total, percentage, student_id))
            conn.commit()
            print("Marks updated successfully.")
        except ValueError:
            print("Invalid input for marks. Marks update skipped.")

    choice = input("Update Fees (Y/N)? ").strip().lower()
    if choice == 'y':
        try:
            fees = float(input("Enter new Fees: "))
            fees_update_sql = "UPDATE students SET fees=? WHERE id=?"
            cursor.execute(fees_update_sql, (fees, student_id))
            conn.commit()
            print("Fees updated successfully.")
        except ValueError:
            print("Invalid input for fees. Fees update skipped.")


def teacher_login():
    """Handles teacher login authentication."""
    print("\n--- Teacher Login ---")
    username = input("Username: ")
    password = getpass("Password: ")
    
    if username == TEACHER_USER and password == TEACHER_PASS:
        print("Teacher login successful.")
        return True
    print("Invalid teacher credentials.")
    return False

def student_login():
    """Handles student login authentication and returns ID on success."""
    print("\n--- Student Login ---")
    try:
        student_id = int(input("Enter Student ID: "))
    except ValueError:
        print("Invalid input for ID.")
        return None
        
    password = getpass("Enter Password: ")

    sql = "SELECT password FROM students WHERE id=?;"
    cursor = conn.cursor()
    cursor.execute(sql, (student_id,))
    result = cursor.fetchone()

    if result and result[0] == password:
        print("Student login successful.")
        return student_id
    
    print("Invalid ID or password.")
    return None

def teacher_menu():
    """Teacher interface menu."""
    while True:
        print("\n--- Teacher Menu ---")
        print("1. Add Student")
        print("2. View All")
        print("3. View by ID")
        print("4. Update Student (Marks/Fees)")
        print("5. Remove Student")
        print("6. Logout")
        
        choice = input("Choice: ").strip()
        
        if choice == '1':
            add_student()
        elif choice == '2':
            view_all_students()
        elif choice == '3':
            try:
                s_id = int(input("Enter ID to view: "))
                view_student_by_id(s_id)
            except ValueError:
                print("Invalid ID input.")
        elif choice == '4':
            update_student()
        elif choice == '5':
            remove_student()
        elif choice == '6':
            print("Logging out...")
            break
        else:
            print("Invalid choice.")

def student_menu():
    """Student interface menu."""
    student_id = student_login()
    if student_id:
        view_student_by_id(student_id)

def main():
    """Main application loop."""
    if not create_connection():
        return
        
    if not setup_database():
        return

    print("Connected to SQLite database successfully.")

    while True:
        print("\n--- Student Management System (SQLite + Python) ---")
        print("1. Teacher Login")
        print("2. Student Login")
        print("3. Exit")
        
        choice = input("Choice: ").strip()
        
        if choice == '1':
            if teacher_login():
                teacher_menu()
        elif choice == '2':
            student_menu()
        elif choice == '3':
            print("Exiting application.")
            break
        else:
            print("Invalid choice.")

    if conn:
        conn.close()

if __name__ == "__main__":
    main()