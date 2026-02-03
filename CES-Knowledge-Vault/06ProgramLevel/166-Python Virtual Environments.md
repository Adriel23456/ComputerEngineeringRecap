---
Theme: Python Virtual Environments
Date created: 2026-01-31 12:00
tags: [ProgramLevel, Python, Tools, Development]
---

## 📚 Idea/Concept

Python virtual environments create isolated spaces for project dependencies, preventing conflicts between packages required by different projects. Understanding virtual environments is essential for professional Python development, reproducible builds, and clean dependency management.

### The Problem
```
Without Virtual Environments:

System Python
├── numpy 1.19 (Project A needs this)
├── numpy 1.24 (Project B needs this) ← CONFLICT!
├── django 2.2
├── django 4.0 ← CONFLICT!
└── hundreds of packages...

All projects share the same packages.
Updating one project can break another.
```

### The Solution
```
With Virtual Environments:

System Python (minimal packages)

Project A (venv_a/)
├── numpy 1.19
├── django 2.2
└── requests 2.25

Project B (venv_b/)
├── numpy 1.24
├── django 4.0
└── flask 2.0

Each project has isolated dependencies.
No conflicts possible.
```

### How Virtual Environments Work

**Directory Structure**:
```
my_project/
├── venv/                    # Virtual environment
│   ├── bin/                 # (Scripts/ on Windows)
│   │   ├── activate         # Activation script
│   │   ├── python           # Symlink to Python
│   │   ├── pip              # Local pip
│   │   └── ...
│   ├── lib/
│   │   └── python3.x/
│   │       └── site-packages/  # Installed packages go here
│   │           ├── numpy/
│   │           ├── requests/
│   │           └── ...
│   └── pyvenv.cfg           # Configuration
├── src/
│   └── main.py
└── requirements.txt
```

**What Activation Does**:
```bash
# Before activation:
$ which python
/usr/bin/python

$ echo $PATH
/usr/bin:/bin:...

# After activation:
$ source venv/bin/activate

$ which python
/home/user/my_project/venv/bin/python

$ echo $PATH
/home/user/my_project/venv/bin:/usr/bin:/bin:...
```

The virtual environment's `bin/` is prepended to PATH, so its Python and pip are found first.

### Creating and Using Virtual Environments

**Standard venv (Built-in)**:
```bash
# Create virtual environment
python -m venv venv

# Activate (Linux/macOS)
source venv/bin/activate

# Activate (Windows)
venv\Scripts\activate

# Your prompt changes:
(venv) $ 

# Install packages (goes to venv only)
pip install numpy pandas requests

# See installed packages
pip list

# Deactivate
deactivate
```

**With virtualenv (Third-party, more features)**:
```bash
pip install virtualenv
virtualenv venv
source venv/bin/activate
```

### Requirements Files

**Freeze current environment**:
```bash
pip freeze > requirements.txt
```

**requirements.txt**:
```
numpy==1.24.0
pandas==2.0.0
requests>=2.28.0,<3.0.0
django~=4.0.0
```

**Install from requirements**:
```bash
pip install -r requirements.txt
```

**Version Specifiers**:
```
package==1.0.0    # Exact version
package>=1.0.0    # Minimum version
package<=2.0.0    # Maximum version
package>=1.0,<2.0 # Range
package~=1.4.0    # Compatible release (>=1.4.0, <1.5.0)
```

### Virtual Environment Workflow
```
1. Create project directory
   $ mkdir my_project && cd my_project

2. Create virtual environment
   $ python -m venv venv

3. Activate it
   $ source venv/bin/activate

4. Install dependencies
   (venv) $ pip install django requests

5. Work on project
   (venv) $ python manage.py runserver

6. Save dependencies
   (venv) $ pip freeze > requirements.txt

7. Deactivate when done
   (venv) $ deactivate

8. Colleague sets up project:
   $ git clone <repo>
   $ cd my_project
   $ python -m venv venv
   $ source venv/bin/activate
   $ pip install -r requirements.txt
```

### Best Practices

**1. Never commit venv to version control**:
```gitignore
# .gitignore
venv/
.venv/
env/
__pycache__/
*.pyc
```

**2. Use descriptive names or standard conventions**:
```bash
venv/        # Most common
.venv/       # Hidden, keeps directory clean
env/         # Also common
```

**3. One virtual environment per project**:
```
projects/
├── web_app/
│   ├── venv/
│   └── requirements.txt
├── data_science/
│   ├── venv/
│   └── requirements.txt
└── scripts/
    ├── venv/
    └── requirements.txt
```

**4. Separate dev and production dependencies**:
```
requirements.txt          # Production
requirements-dev.txt      # Development (includes testing, linting)
```

### Modern Alternatives

| Tool | Description |
|------|-------------|
| **venv** | Built-in, simple, sufficient for most cases |
| **virtualenv** | More features, faster creation |
| **conda** | Environment + package manager, great for data science |
| **pipenv** | Combines pip + virtualenv + Pipfile |
| **poetry** | Modern dependency management + packaging |

**Poetry Example**:
```bash
poetry new my_project
cd my_project
poetry add numpy pandas
poetry install
poetry shell  # Activates environment
```

## 📌 Key Points (Optional)
- Virtual environments isolate project dependencies
- Activation modifies PATH to use local Python/pip
- Always use requirements.txt for reproducibility
- Never commit venv/ to version control
- One virtual environment per project

## 🖼️ Recommended Image
- Diagram showing isolated environments with different package versions

## 🔗 Connections
- [[165-Python Fundamentals]]
- [[169-Programming Paradigms Overview]]