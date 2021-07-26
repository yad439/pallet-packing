import time
from typing import Any, Callable, List, Tuple

from common import Item


def print_efficiency(pallet_width: int, pallet_height: int, items: List[Item],
                     algorithm: Callable[[int, int, List[Item]], Tuple[int, Any]]) -> None:
    start_time = time.time()
    result = algorithm(pallet_width, pallet_height, items)
    end_time = time.time()
    print('Execution took:', end_time - start_time, 'seconds\nResult is:', result[0], '-',
          '{:.2%}'.format(result[0] / (pallet_width * pallet_height)), 'of pallet is free')
