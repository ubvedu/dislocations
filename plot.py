import math

import matplotlib.pyplot as plt
import numpy as np
import pandas as pd

# single

single = pd.read_csv('out/single.csv')
crystal_size = np.array(single.loc[:, 'crystal size'])
steps_single = np.array(single.loc[:, 'mean steps'])
k_single, _ = np.polyfit(crystal_size ** 2, steps_single, 1)
sigma_k_single = np.sqrt(((steps_single / crystal_size ** 2 - k_single) ** 2).mean())

multiple = pd.read_csv('out/multiple.csv')
num_dis = np.array(multiple.loc[:, 'num dislocations'])
steps_multiple = np.array(multiple.loc[:, 'steps'])

chunks = 100
crystal_size_chunked = np.array([array.mean() for array in np.array_split(crystal_size, chunks)])
steps_multiple_chunked = np.array([array.mean() for array in np.array_split(steps_multiple, chunks)])
k_multiple, _ = np.polyfit(crystal_size_chunked ** 2, steps_multiple_chunked, 1)
sigma_k_multiple = np.sqrt(((steps_multiple_chunked / crystal_size_chunked ** 2 - k_multiple) ** 2).mean())

plt.figure(figsize=(8, 4))
plt.grid(color='#e0e0e0', zorder=0)
plt.xlabel('crystal area')
plt.ylabel('steps before stopping')

plt.scatter(crystal_size ** 2, steps_single, c='#6929c4', s=3, label='single dislocation (100 simulations each)',
            zorder=2)
plt.plot(crystal_size ** 2, k_single * crystal_size ** 2, color='#6929c480', lw=2,
         label='single dislocation mean line', zorder=2)
plt.plot(crystal_size ** 2, (k_single + sigma_k_single) * crystal_size ** 2, color='#6929c480', lw=2,
         zorder=2, linestyle=(0, (5, 5)))
plt.plot(crystal_size ** 2, (k_single - sigma_k_single) * crystal_size ** 2, color='#6929c480', lw=2,
         zorder=2, linestyle=(0, (5, 5)))

plt.scatter(crystal_size_chunked ** 2, steps_multiple_chunked, c='#009d9a', s=3,
            label='multiple dislocations (1 simulation each)', zorder=2)
plt.plot(crystal_size ** 2, k_multiple * crystal_size ** 2, color='#009d9a80', lw=2,
         label='multiple dislocations mean line', zorder=2)
plt.plot(crystal_size ** 2, (k_multiple + sigma_k_multiple) * crystal_size ** 2, color='#009d9a80', lw=2,
         zorder=2, linestyle=(0, (5, 5)))
plt.plot(crystal_size ** 2, (k_multiple - sigma_k_multiple) * crystal_size ** 2, color='#009d9a80', lw=2,
         zorder=2, linestyle=(0, (5, 5)))

plt.legend()
plt.savefig('out/steps-by-crystal-area.png')

areas_to_steps = np.stack([num_dis / crystal_size ** 2, steps_multiple], axis=1)
areas_to_steps_sorted = areas_to_steps[areas_to_steps[:, 0].argsort()[::-1]]
areas_to_steps_sorted_chunked = np.array([[b.mean() for b in a.swapaxes(0, 1)]
                                          for a in np.array_split(areas_to_steps_sorted, chunks)]).swapaxes(0, 1)
# m = areas_to_steps_sorted_chunked[0].max()
# k_areas_to_steps, _ = np.polyfit(m - areas_to_steps_sorted_chunked[0], areas_to_steps_sorted_chunked[1], 1)
# sigma_k_areas_to_steps = np.sqrt(
#     ((areas_to_steps_sorted_chunked[1][:-1] / (m - areas_to_steps_sorted_chunked[0][:-1]) - k_areas_to_steps) ** 2).mean())

plt.figure(figsize=(8, 4))
plt.grid(color='#e0e0e0', zorder=0)
plt.xlabel('area ratio')
plt.ylabel('steps before stopping')

plt.scatter(areas_to_steps_sorted_chunked[0], areas_to_steps_sorted_chunked[1], c='#009d9a', s=2,
            label='multiple dislocations (1 simulation each)', zorder=2)
# plt.plot(areas_to_steps_sorted_chunked[0], k_areas_to_steps * (m - areas_to_steps_sorted_chunked[0]), color='#009d9a80',
#          lw=2, label='multiple dislocations mean line', zorder=2)
# plt.plot(areas_to_steps_sorted_chunked[0],
#          (k_areas_to_steps + sigma_k_areas_to_steps) * (m - areas_to_steps_sorted_chunked[0]), color='#009d9a80',
#          lw=2, zorder=2, linestyle=(0, (5, 5)))
# plt.plot(areas_to_steps_sorted_chunked[0],
#          (k_areas_to_steps - sigma_k_areas_to_steps) * (m - areas_to_steps_sorted_chunked[0]), color='#009d9a80',
#          lw=2, zorder=2, linestyle=(0, (5, 5)))

plt.legend()
plt.savefig('out/steps-by-area-ratio.png')
