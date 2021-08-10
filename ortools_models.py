from typing import Collection, List, Optional, Tuple, Union

from ortools.linear_solver import pywraplp

from common import Item, Position


def task_one_model(pallet_width: int, pallet_height: int, items: Collection[Item], *, limit: int = 0,
                   backend: str = 'SCIP'):
    solver = _build_simple_model(pallet_width, pallet_height, items, backend)

    solver.EnableOutput()

    if limit != 0:
        solver.SetTimeLimit(limit)

    status = solver.Solve()

    n = len(items)
    zv = [bool(solver.LookupVariable(f'z{i}').solution_value()) for i in range(n)]
    xv = [solver.LookupVariable(f'x{i}').solution_value() for i in range(n)]
    yv = [solver.LookupVariable(f'y{i}').solution_value() for i in range(n)]
    rv = [bool(solver.LookupVariable(f'r{i}').solution_value()) for i in range(n)]

    return status, solver.Objective().Value(), solver.Objective().BestBound(), [
            Position(xv[i], yv[i], rv[i]) if zv[i] else None for i in range(n)]


def task_one_model_adapter(pallet_width: int, pallet_height: int, items: List[Item], time_limit: int = 60 * 1000) \
        -> Tuple[int, List[Optional[Position]]]:
    result = task_one_model(pallet_width, pallet_height, items, limit = time_limit)
    return int(result[1]), result[3]


def task_two_model(pallet_width: int, pallet_height: int, items: List[Item], x_tolerance: Union[float, int],
                   y_tolerance: Union[float, int], *, limit: int = 0, backend: str = 'SCIP'):
    solver = _build_simple_model(pallet_width, pallet_height, items, backend)
    n = len(items)
    h = list(map(lambda i: i.height, items))
    w = list(map(lambda i: i.width, items))
    big_num = sum(i.width + i.height for i in items)
    x = [solver.LookupVariable(f'x{i}') for i in range(n)]
    y = [solver.LookupVariable(f'y{i}') for i in range(n)]
    z = [solver.LookupVariable(f'z{i}') for i in range(n)]
    r = [solver.LookupVariable(f'r{i}') for i in range(n)]
    cx = [solver.NumVar(0, solver.infinity(), f'x{i}') for i in range(n)]
    cy = [solver.NumVar(0, solver.infinity(), f'y{i}') for i in range(n)]

    for i in range(n):
        solver.Add(cx[i] <= big_num * z[i])
        solver.Add(cy[i] <= big_num * z[i])
        solver.Add(cx[i] <= x[i] + w[i] / 2 * (1 - r[i]) + h[i] / 2 * r[i])
        solver.Add(cy[i] <= y[i] + h[i] / 2 * (1 - r[i]) + w[i] / 2 * r[i])
        solver.Add(cx[i] >= x[i] + w[i] / 2 * (1 - r[i]) + h[i] / 2 * r[i] - big_num * (1 - z[i]))
        solver.Add(cx[i] >= y[i] + h[i] / 2 * (1 - r[i]) + w[i] / 2 * r[i] - big_num * (1 - z[i]))
        for j in range(n):
            if i != j:
                solver.Add(sum(items[i].mass * cx[i] for i in range(n)) <= (pallet_width / 2 + x_tolerance) * sum(
                        items[i].mass * z[i] for i in range(n)))
                solver.Add(sum(items[i].mass * cx[i] for i in range(n)) >= (pallet_width / 2 - x_tolerance) * sum(
                        items[i].mass * z[i] for i in range(n)))
                solver.Add(sum(items[i].mass * cy[i] for i in range(n)) <= (pallet_height / 2 + y_tolerance) * sum(
                        items[i].mass * z[i] for i in range(n)))
                solver.Add(sum(items[i].mass * cy[i] for i in range(n)) >= (pallet_height / 2 - y_tolerance) * sum(
                        items[i].mass * z[i] for i in range(n)))

    solver.EnableOutput()

    if limit != 0:
        solver.SetTimeLimit(limit)

    status = solver.Solve()

    zv = [bool(t.solution_value()) for t in z]
    xv = [t.solution_value() for t in x]
    yv = [t.solution_value() for t in y]
    rv = [bool(t.solution_value()) for t in r]

    return status, solver.Objective().Value(), solver.Objective().BestBound(), [
            Position(xv[i], yv[i], rv[i]) if zv[i] else None for i in range(n)]


def _build_simple_model(pallet_width: int, pallet_height: int, items: Collection[Item],
                        backend: str) -> pywraplp.Solver:
    solver = pywraplp.Solver.CreateSolver(backend)
    big_num = sum(i.width + i.height for i in items)
    n = len(items)
    h = list(map(lambda i: i.height, items))
    w = list(map(lambda i: i.width, items))
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
