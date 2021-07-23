from ortools.linear_solver import pywraplp


def taskOneModel(H, W, items, *, limit=0):
    solver = pywraplp.Solver.CreateSolver('SCIP')
    B = sum(i[0] + i[1] for i in items)
    n = len(items)
    h = list(map(lambda i: i[1], items))
    w = list(map(lambda i: i[0], items))

    x = [solver.NumVar(0, solver.infinity(), f'x{i}') for i in range(n)]
    y = [solver.NumVar(0, solver.infinity(), f'y{i}') for i in range(n)]
    l = [[solver.BoolVar(f'l{i},{j}') for j in range(n)] for i in range(n)]
    b = [[solver.BoolVar(f'b{i},{j}') for j in range(n)] for i in range(n)]
    r = [solver.BoolVar(f'r{i}') for i in range(n)]
    z = [solver.BoolVar(f'z{i}') for i in range(n)]

    for i in range(n):
        solver.Add(x[i] + w[i] * (1 - r[i]) + h[i] * r[i] <= W + (1 - z[i]) * B)
        solver.Add(y[i] + h[i] * (1 - r[i]) + w[i] * r[i] <= H + (1 - z[i]) * B)
        for j in range(n):
            if i != j:
                solver.Add(x[i] + w[i] * (1 - r[i]) + h[i] * r[i] <= x[j] + (1 - l[i][j]) * B)
                solver.Add(y[i] + h[i] * (1 - r[i]) + w[i] * r[i] <= y[j] + (1 - b[i][j]) * B)
                solver.Add(l[i][j] + l[j][i] + b[i][j] + b[j][i] >= 1)

    solver.Minimize(H * W - sum(h[i] * w[i] * z[i] for i in range(n)))

    if limit != 0:
        solver.SetTimeLimit(limit)

    solver.EnableOutput()

    status = solver.Solve()

    zv = [bool(t.solution_value()) for t in z]
    xv = [t.solution_value() for t in x]
    yv = [t.solution_value() for t in y]
    rv = [bool(t.solution_value()) for t in r]

    return status, solver.Objective().Value(), solver.Objective().BestBound(), list(zip(zv, xv, yv, rv))


def taskTwoModel(H, W, items, xtol, ytol, *, limit=0):
    solver = pywraplp.Solver.CreateSolver('SCIP')
    B = sum(i[0] + i[1] for i in items)
    n = len(items)
    h = list(map(lambda i: i[1], items))
    w = list(map(lambda i: i[0], items))

    x = [solver.NumVar(0, solver.infinity(), f'x{i}') for i in range(n)]
    y = [solver.NumVar(0, solver.infinity(), f'y{i}') for i in range(n)]
    l = [[solver.BoolVar(f'l{i},{j}') for j in range(n)] for i in range(n)]
    b = [[solver.BoolVar(f'b{i},{j}') for j in range(n)] for i in range(n)]
    r = [solver.BoolVar(f'r{i}') for i in range(n)]
    z = [solver.BoolVar(f'z{i}') for i in range(n)]
    cx = [solver.NumVar(0, solver.infinity(), f'x{i}') for i in range(n)]
    cy = [solver.NumVar(0, solver.infinity(), f'y{i}') for i in range(n)]

    for i in range(n):
        solver.Add(x[i] + w[i] * (1 - r[i]) + h[i] * r[i] <= W + (1 - z[i]) * B)
        solver.Add(y[i] + h[i] * (1 - r[i]) + w[i] * r[i] <= H + (1 - z[i]) * B)
        solver.Add(cx[i] <= B * z[i])
        solver.Add(cy[i] <= B * z[i])
        solver.Add(cx[i] <= x[i])
        solver.Add(cy[i] <= y[i])
        solver.Add(cx[i] >= x[i] - B * (1 - z[i]))
        solver.Add(cy[i] >= y[i] - B * (1 - z[i]))
        for j in range(n):
            if i != j:
                solver.Add(x[i] + w[i] * (1 - r[i]) + h[i] * r[i] <= x[j] + (1 - l[i][j]) * B)
                solver.Add(y[i] + h[i] * (1 - r[i]) + w[i] * r[i] <= y[j] + (1 - b[i][j]) * B)
                solver.Add(l[i][j] + l[j][i] + b[i][j] + b[j][i] >= 1)
    solver.Add(sum(items[i][2] * cx[i] for i in range(n)) <= (W / 2 + xtol) * sum(items[i][2] * z[i] for i in range(n)))
    solver.Add(sum(items[i][2] * cx[i] for i in range(n)) >= (W / 2 - xtol) * sum(items[i][2] * z[i] for i in range(n)))
    solver.Add(sum(items[i][2] * cy[i] for i in range(n)) <= (W / 2 + ytol) * sum(items[i][2] * z[i] for i in range(n)))
    solver.Add(sum(items[i][2] * cy[i] for i in range(n)) >= (W / 2 - ytol) * sum(items[i][2] * z[i] for i in range(n)))

    solver.Minimize(H * W - sum(h[i] * w[i] * z[i] for i in range(n)))

    if limit != 0:
        solver.SetTimeLimit(limit)

    solver.EnableOutput()

    status = solver.Solve()

    zv = [bool(t.solution_value()) for t in z]
    xv = [t.solution_value() for t in x]
    yv = [t.solution_value() for t in y]
    rv = [bool(t.solution_value()) for t in r]

    return status, solver.Objective().Value(), solver.Objective().BestBound(), list(zip(zv, xv, yv, rv))
