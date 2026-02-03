---
Theme: SQL and Relational Databases
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Databases, SQL, DataModeling]
---

## 📚 Idea/Concept

Relational databases organize data into tables with rows and columns, using SQL (Structured Query Language) for querying and manipulation. Based on relational algebra, they enforce data integrity through constraints and relationships. Understanding relational databases is fundamental for data persistence in virtually all software systems.

### Relational Model Concepts

**Table (Relation)**: Collection of related data entries
**Row (Tuple)**: Single record
**Column (Attribute)**: Field with specific data type
**Primary Key**: Uniquely identifies each row
**Foreign Key**: References primary key in another table
```
Students Table:
┌────────┬───────────┬─────────┬─────────────────┐
│ id(PK) │   name    │   age   │     major       │
├────────┼───────────┼─────────┼─────────────────┤
│   1    │   Alice   │   20    │ Computer Science│
│   2    │   Bob     │   22    │ Mathematics     │
│   3    │   Carol   │   21    │ Computer Science│
└────────┴───────────┴─────────┴─────────────────┘

Enrollments Table:
┌────────┬────────────┬─────────────┐
│ id(PK) │ student_id │ course_code │
├────────┼────────────┼─────────────┤
│   1    │     1      │   CS101     │
│   2    │     1      │   MA201     │
│   3    │     2      │   MA201     │
└────────┴────────────┴─────────────┘
        (FK→Students)
```

### SQL Fundamentals

#### Data Definition Language (DDL)
```sql
-- Create table
CREATE TABLE Students (
    id INT PRIMARY KEY AUTO_INCREMENT,
    name VARCHAR(100) NOT NULL,
    age INT CHECK (age >= 0),
    major VARCHAR(100),
    email VARCHAR(255) UNIQUE,
    created_at TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);

-- Create related table with foreign key
CREATE TABLE Enrollments (
    id INT PRIMARY KEY AUTO_INCREMENT,
    student_id INT NOT NULL,
    course_code VARCHAR(10) NOT NULL,
    grade DECIMAL(3, 2),
    FOREIGN KEY (student_id) REFERENCES Students(id)
        ON DELETE CASCADE
);

-- Modify table
ALTER TABLE Students ADD COLUMN gpa DECIMAL(3, 2);
ALTER TABLE Students DROP COLUMN age;

-- Delete table
DROP TABLE IF EXISTS Students;
```

#### Data Manipulation Language (DML)
```sql
-- Insert data
INSERT INTO Students (name, age, major)
VALUES ('Alice', 20, 'Computer Science');

INSERT INTO Students (name, age, major)
VALUES 
    ('Bob', 22, 'Mathematics'),
    ('Carol', 21, 'Computer Science');

-- Update data
UPDATE Students 
SET major = 'Data Science' 
WHERE name = 'Alice';

-- Delete data
DELETE FROM Students WHERE age < 18;
```

#### Queries (SELECT)
```sql
-- Basic select
SELECT name, major FROM Students;

-- Filtering
SELECT * FROM Students 
WHERE age >= 21 AND major = 'Computer Science';

-- Sorting
SELECT * FROM Students ORDER BY age DESC, name ASC;

-- Aggregation
SELECT major, COUNT(*) as count, AVG(age) as avg_age
FROM Students
GROUP BY major
HAVING COUNT(*) > 1;

-- Distinct values
SELECT DISTINCT major FROM Students;

-- Limiting results
SELECT * FROM Students LIMIT 10 OFFSET 20;
```

### Joins

Combine rows from multiple tables:
```sql
-- Inner Join: Only matching rows
SELECT s.name, e.course_code
FROM Students s
INNER JOIN Enrollments e ON s.id = e.student_id;

-- Left Join: All from left, matching from right
SELECT s.name, e.course_code
FROM Students s
LEFT JOIN Enrollments e ON s.id = e.student_id;
-- Students without enrollments show NULL for course_code

-- Right Join: All from right, matching from left
-- Full Outer Join: All from both tables
```

**Visual Join Types**:
```
       A          B
    ┌─────┐    ┌─────┐
    │     │    │     │
    │  ┌──┼────┼──┐  │
    │  │  │    │  │  │
    │  └──┼────┼──┘  │
    │     │    │     │
    └─────┘    └─────┘

INNER: Center overlap only
LEFT:  All of A + overlap
RIGHT: Overlap + all of B
FULL:  All of A + all of B
```

### Keys and Constraints
```sql
-- Primary Key: Unique identifier
CREATE TABLE Products (
    product_id INT PRIMARY KEY,
    ...
);

-- Composite Primary Key
CREATE TABLE OrderItems (
    order_id INT,
    product_id INT,
    quantity INT,
    PRIMARY KEY (order_id, product_id)
);

-- Foreign Key: References another table
CREATE TABLE Orders (
    id INT PRIMARY KEY,
    customer_id INT,
    FOREIGN KEY (customer_id) REFERENCES Customers(id)
);

-- Constraints
NOT NULL        -- Cannot be empty
UNIQUE          -- No duplicate values
CHECK (age > 0) -- Custom validation
DEFAULT value   -- Default if not specified
```

### Normalization

Organizing data to reduce redundancy:

**1NF**: Atomic values, no repeating groups
```
❌ student_courses = "CS101, MA201, PH101"
✓ Separate Enrollments table with one course per row
```

**2NF**: 1NF + no partial dependencies on composite key
```
❌ OrderItems(order_id, product_id, product_name, qty)
   product_name depends only on product_id
✓ Move product_name to Products table
```

**3NF**: 2NF + no transitive dependencies
```
❌ Students(id, name, dept_id, dept_name)
   dept_name depends on dept_id, not on id
✓ Move dept_name to Departments table
```

### Indexes

Speed up queries at cost of write performance:
```sql
-- Create index
CREATE INDEX idx_students_major ON Students(major);

-- Composite index
CREATE INDEX idx_name_age ON Students(name, age);

-- Unique index
CREATE UNIQUE INDEX idx_email ON Students(email);

-- View execution plan
EXPLAIN SELECT * FROM Students WHERE major = 'CS';
```

### Transactions

Ensure data integrity with ACID properties:
```sql
BEGIN TRANSACTION;

UPDATE Accounts SET balance = balance - 100 WHERE id = 1;
UPDATE Accounts SET balance = balance + 100 WHERE id = 2;

-- If both succeed
COMMIT;

-- If error occurs
ROLLBACK;
```

**ACID Properties**:
- **Atomicity**: All or nothing
- **Consistency**: Valid state to valid state
- **Isolation**: Concurrent transactions don't interfere
- **Durability**: Committed changes persist

## 📌 Key Points (Optional)
- Tables, rows, columns form the relational structure
- Primary keys uniquely identify rows; foreign keys create relationships
- JOIN combines data from multiple tables
- Normalization reduces redundancy (1NF, 2NF, 3NF)
- ACID properties ensure transaction reliability

## 🖼️ Recommended Image
- Entity-Relationship diagram showing table relationships with keys

## 🔗 Connections
- [[171-Data Structures Overview]]
- [[176-Algorithm Complexity Analysis]]