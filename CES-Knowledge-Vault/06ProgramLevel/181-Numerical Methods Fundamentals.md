---
Theme: Numerical Methods Fundamentals
Date created: 2026-01-31 12:00
tags: [ProgramLevel, NumericalMethods, Mathematics, Algorithms]
---

## 📚 Idea/Concept

Numerical methods are algorithms for solving mathematical problems approximately using arithmetic operations. When analytical solutions are impossible or impractical, numerical methods provide approximate solutions to equations, integrals, derivatives, and differential equations. These techniques are fundamental to scientific computing, engineering simulation, and computational mathematics.

### Why Numerical Methods?

Many problems have no closed-form solution:
- Most differential equations
- Many integrals (e.g., ∫e^(-x²)dx)
- Roots of complex polynomials
- Large systems of equations

### Core Areas
```
Numerical Methods
├── Root Finding
│   ├── Bisection Method
│   ├── Newton-Raphson
│   └── Secant Method
├── Linear Systems
│   ├── Gaussian Elimination
│   ├── LU Decomposition
│   └── Iterative Methods (Jacobi, Gauss-Seidel)
├── Interpolation
│   ├── Lagrange
│   └── Splines
├── Integration (Quadrature)
│   ├── Trapezoidal Rule
│   ├── Simpson's Rule
│   └── Gaussian Quadrature
└── Differential Equations
    ├── Euler Method
    └── Runge-Kutta
```

---

### Root Finding

**Problem**: Find x where f(x) = 0

#### Bisection Method

Repeatedly halve interval containing root:
```python
def bisection(f, a, b, tol=1e-6):
    """
    Requires: f(a) and f(b) have opposite signs
    Guarantees: Convergence (if root exists in [a,b])
    Rate: Linear (one bit of accuracy per iteration)
    """
    while (b - a) / 2 > tol:
        mid = (a + b) / 2
        if f(mid) == 0:
            return mid
        elif f(a) * f(mid) < 0:
            b = mid
        else:
            a = mid
    return (a + b) / 2

# Example: Find √2 (root of f(x) = x² - 2)
root = bisection(lambda x: x**2 - 2, 1, 2)  # ≈ 1.414213
```

#### Newton-Raphson Method

Use tangent line to approximate root:
```
x_{n+1} = x_n - f(x_n) / f'(x_n)
```
```python
def newton_raphson(f, df, x0, tol=1e-6, max_iter=100):
    """
    Requires: Good initial guess, f'(x) ≠ 0 near root
    Rate: Quadratic (doubles correct digits each iteration)
    """
    x = x0
    for _ in range(max_iter):
        fx = f(x)
        dfx = df(x)
        if abs(dfx) < 1e-12:
            raise ValueError("Derivative too small")
        
        x_new = x - fx / dfx
        
        if abs(x_new - x) < tol:
            return x_new
        x = x_new
    
    raise ValueError("Did not converge")

# Example: Find √2
f = lambda x: x**2 - 2
df = lambda x: 2*x
root = newton_raphson(f, df, 1.5)  # Converges in ~4 iterations
```

**Geometric Interpretation**:
```
     │    f(x)
     │   ╱
     │  ╱
     │ ╱
     │╱────────── tangent line
   ──┼──────────────
     │  x₁  x₀
     
x₁ is where tangent at x₀ crosses x-axis
```

---

### Linear Systems

**Problem**: Solve Ax = b

#### Gaussian Elimination

Reduce to upper triangular form, then back-substitute:
```python
def gaussian_elimination(A, b):
    """
    Solve Ax = b using Gaussian elimination
    Complexity: O(n³)
    """
    n = len(b)
    # Augmented matrix [A|b]
    M = [A[i] + [b[i]] for i in range(n)]
    
    # Forward elimination
    for i in range(n):
        # Partial pivoting (for stability)
        max_row = max(range(i, n), key=lambda r: abs(M[r][i]))
        M[i], M[max_row] = M[max_row], M[i]
        
        for j in range(i + 1, n):
            factor = M[j][i] / M[i][i]
            for k in range(i, n + 1):
                M[j][k] -= factor * M[i][k]
    
    # Back substitution
    x = [0] * n
    for i in range(n - 1, -1, -1):
        x[i] = M[i][n]
        for j in range(i + 1, n):
            x[i] -= M[i][j] * x[j]
        x[i] /= M[i][i]
    
    return x
```

#### Iterative Methods

For large sparse systems, direct methods are too slow.

**Gauss-Seidel**:
```python
def gauss_seidel(A, b, x0, tol=1e-6, max_iter=1000):
    """
    Iteratively refine solution
    Converges if A is diagonally dominant
    """
    n = len(b)
    x = x0.copy()
    
    for _ in range(max_iter):
        x_old = x.copy()
        
        for i in range(n):
            sigma = sum(A[i][j] * x[j] for j in range(n) if j != i)
            x[i] = (b[i] - sigma) / A[i][i]
        
        # Check convergence
        if max(abs(x[i] - x_old[i]) for i in range(n)) < tol:
            return x
    
    return x
```

---

### Numerical Integration

**Problem**: Evaluate ∫[a,b] f(x) dx

#### Trapezoidal Rule

Approximate area with trapezoids:
```python
def trapezoidal(f, a, b, n):
    """
    Error: O(h²) where h = (b-a)/n
    """
    h = (b - a) / n
    result = 0.5 * (f(a) + f(b))
    
    for i in range(1, n):
        result += f(a + i * h)
    
    return result * h

# ∫[0,1] x² dx = 1/3 ≈ 0.333...
integral = trapezoidal(lambda x: x**2, 0, 1, 100)
```

#### Simpson's Rule

Approximate with parabolas (more accurate):
```python
def simpson(f, a, b, n):
    """
    Requires: n must be even
    Error: O(h⁴) - much better than trapezoidal
    """
    if n % 2 != 0:
        n += 1
    
    h = (b - a) / n
    result = f(a) + f(b)
    
    for i in range(1, n):
        coef = 4 if i % 2 == 1 else 2
        result += coef * f(a + i * h)
    
    return result * h / 3
```

---

### Differential Equations

**Problem**: Solve dy/dx = f(x, y) with initial condition y(x₀) = y₀

#### Euler's Method

Simplest ODE solver - follow the slope:
```python
def euler(f, x0, y0, x_end, h):
    """
    f(x, y) = dy/dx
    h = step size
    Error: O(h) per step
    """
    x, y = x0, y0
    points = [(x, y)]
    
    while x < x_end:
        y = y + h * f(x, y)  # y_{n+1} = y_n + h * f(x_n, y_n)
        x = x + h
        points.append((x, y))
    
    return points

# Example: dy/dx = y, y(0) = 1  (solution: y = e^x)
points = euler(lambda x, y: y, 0, 1, 2, 0.1)
```

**Geometric Interpretation**:
```
    y│      actual curve
     │         ╱
     │       ╱
     │     ╱ ← Euler follows tangent
     │   ╱───────
     │ ╱
     └──────────────x
```

#### Runge-Kutta (RK4)

Fourth-order method - much more accurate:
```python
def rk4(f, x0, y0, x_end, h):
    """
    Classic 4th order Runge-Kutta
    Error: O(h⁴) per step - excellent accuracy
    """
    x, y = x0, y0
    points = [(x, y)]
    
    while x < x_end:
        k1 = h * f(x, y)
        k2 = h * f(x + h/2, y + k1/2)
        k3 = h * f(x + h/2, y + k2/2)
        k4 = h * f(x + h, y + k3)
        
        y = y + (k1 + 2*k2 + 2*k3 + k4) / 6
        x = x + h
        points.append((x, y))
    
    return points
```

---

### Error and Stability

**Truncation Error**: From approximating mathematical operations
**Round-off Error**: From finite precision arithmetic

**Stability**: Small input changes → small output changes
```
Condition Number κ(A) for linear systems:
- κ ≈ 1: Well-conditioned (stable)
- κ >> 1: Ill-conditioned (unstable)

Example of ill-conditioning:
[1.000  1.000] [x]   [2]
[1.000  1.001] [y] = [2]

Tiny change in coefficients → huge change in solution
```

## 📌 Key Points (Optional)
- Newton-Raphson: Quadratic convergence but needs derivative and good initial guess
- Gaussian elimination: O(n³), use partial pivoting for stability
- Simpson's rule: O(h⁴) error, much better than trapezoidal
- RK4: Standard ODE solver, O(h⁴) accuracy
- Always consider stability and conditioning of problems

## 🖼️ Recommended Image
- Comparison of Euler vs RK4 approximations to exact ODE solution

## 🔗 Connections
- [[176-Algorithm Complexity Analysis]]
- [[162-C Language Fundamentals and Data Types]]
- [[167-Recursion Fundamentals]]