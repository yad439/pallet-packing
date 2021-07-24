from ortools.linear_solver import pywraplp


def task_one_model(pallet_width, pallet_height, items, *, limit=0, backend='SCIP'):
    solver = _build_simple_model(pallet_width, pallet_height, items, backend)

    if limit != 0:
        solver.SetTimeLimit(limit)

    status = solver.Solve()

    n = len(items)
    zv = [bool(solver.LookupVariable(f'z{i}').solution_value()) for i in range(n)]
    xv = [solver.LookupVariable(f'x{i}').solution_value() for i in range(n)]
    yv = [solver.LookupVariable(f'y{i}').solution_value() for i in range(n)]
    rv = [bool(solver.LookupVariable(f'r{i}').solution_value()) for i in range(n)]

    return status, solver.Objective().Value(), solver.Objective().BestBound(), list(zip(zv, xv, yv, rv))


# wrong model
def task_two_model(pallet_width, pallet_height, items, x_tolerance, y_tolerance, *, limit=0, backend='SCIP'):
    solver = _build_simple_model(pallet_width, pallet_height, items, backend)
    n = len(items)
    big_num = sum(i[0] + i[1] for i in items)
    x = [solver.LookupVariable(f'x{i}') for i in range(n)]
    y = [solver.LookupVariable(f'y{i}') for i in range(n)]
    z = [solver.LookupVariable(f'z{i}') for i in range(n)]
    r = [solver.LookupVariable(f'r{i}') for i in range(n)]
    cx = [solver.NumVar(0, solver.infinity(), f'x{i}') for i in range(n)]
    cy = [solver.NumVar(0, solver.infinity(), f'y{i}') for i in range(n)]

    for i in range(n):
        solver.Add(cx[i] <= big_num * z[i])
        solver.Add(cy[i] <= big_num * z[i])
        solver.Add(cx[i] <= x[i])
        solver.Add(cy[i] <= y[i])
        solver.Add(cx[i] >= x[i] - big_num * (1 - z[i]))
        solver.Add(cy[i] >= y[i] - big_num * (1 - z[i]))
        for j in range(n):
            if i != j:
                solver.Add(sum(items[i][2] * cx[i] for i in range(n)) <= (pallet_width / 2 + x_tolerance) * sum(
                        items[i][2] * z[i] for i in range(n)))
                solver.Add(sum(items[i][2] * cx[i] for i in range(n)) >= (pallet_width / 2 - x_tolerance) * sum(
                        items[i][2] * z[i] for i in range(n)))
                solver.Add(sum(items[i][2] * cy[i] for i in range(n)) <= (pallet_width / 2 + y_tolerance) * sum(
                        items[i][2] * z[i] for i in range(n)))
                solver.Add(sum(items[i][2] * cy[i] for i in range(n)) >= (pallet_width / 2 - y_tolerance) * sum(
                        items[i][2] * z[i] for i in range(n)))

    if limit != 0:
        solver.SetTimeLimit(limit)

    status = solver.Solve()

    zv = [bool(t.solution_value()) for t in z]
    xv = [t.solution_value() for t in x]
    yv = [t.solution_value() for t in y]
    rv = [bool(t.solution_value()) for t in r]

    return status, solver.Objective().Value(), solver.Objective().BestBound(), list(zip(zv, xv, yv, rv))


def _build_simple_model(pallet_width, pallet_height, items, backend):
    solver = pywraplp.Solver.CreateSolver(backend)
    big_num = sum(i[0] + i[1] for i in items)
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
        solver.Add(x[i] + w[i] * (1 - r[i]) + h[i] * r[i] <= pallet_width + (1 - z[i]) * big_num)
        solver.Add(y[i] + h[i] * (1 - r[i]) + w[i] * r[i] <= pallet_height + (1 - z[i]) * big_num)
        for j in range(n):
            if i != j:
                solver.Add(x[i] + w[i] * (1 - r[i]) + h[i] * r[i] <= x[j] + (1 - l[i][j]) * big_num)
                solver.Add(y[i] + h[i] * (1 - r[i]) + w[i] * r[i] <= y[j] + (1 - b[i][j]) * big_num)
                solver.Add(l[i][j] + l[j][i] + b[i][j] + b[j][i] >= 1)
    solver.Minimize(pallet_height * pallet_width - sum(h[i] * w[i] * z[i] for i in range(n)))
    return solver
