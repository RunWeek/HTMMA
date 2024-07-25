import matplotlib.pyplot as plt
import numpy as np
import matplotlib

# 设置字体属性
font = {'family': 'serif',
        'color': 'black',
        'weight': 'normal',
        'size': 14,
        }

matplotlib.use('Agg')

# 从文本文件中读取数据
data = np.genfromtxt('./virtualaddr_memcached_1000_bak.txt', dtype=None, encoding=None, delimiter=' ', names=['prefix', 'virtual_addr', 'tid', 'timestamps'])

# 提取地址部分、tid，并转换为整数
addresses = [int(addr.split(b'=')[1], 16) for addr in data['virtual_addr']]
tids = data['tid']

# 计算基准地址
base_address = addresses[0]

# 获取最小地址，以便偏移量不为负数
min_address = min(addresses)

# 计算每个地址相对于基准地址的偏移量并将其转换为4KB页面大小的块
page_size_bytes = 4 * 1024 
addresses_in_pages = [(addr - min_address) // page_size_bytes for addr in addresses]

# 统计每个页面的访问次数和对应的tid
page_counts = {}
page_tids = {}
for idx, page in enumerate(addresses_in_pages):
    if page in page_counts:
        page_counts[page] += 1
        page_tids[page].append(tids[idx])
    else:
        page_counts[page] = 1
        page_tids[page] = [tids[idx]]

# 最大访问次数，用于归一化颜色映射
max_count = max(page_counts.values())

# 创建时间数据（千分之一秒为间隔），采样频率默认为1000Hz
time_interval = 0.001  # 1毫秒
time_points = np.arange(len(addresses_in_pages)) * time_interval

# 使用对数尺度来调整颜色映射
log_counts = [np.log(count) if count > 0 else 0 for count in page_counts.values()]

# 自定义颜色映射，将低访问次数映射到浅色，高访问次数映射到深色
cmap = plt.get_cmap('viridis')
norm = matplotlib.colors.Normalize(vmin=0, vmax=max_count)
colors = [cmap(norm(page_counts[page])) for page in addresses_in_pages]

# 创建可视化图表
plt.figure(figsize=(10, 6))

# 创建一个包含每个页面内存访问的离散点，颜色深浅根据页面访问次数映射
plt.scatter(time_points, addresses_in_pages, c=colors, s=1, alpha=0.1)

# 设置X轴和Y轴标签及标题的字体属性
plt.xlabel('Time (seconds)', fontdict=font)
plt.ylabel('Page', fontdict=font)
plt.title('Memory Pages Access (Color-coded by Access Count)', fontdict=font)

plt.grid(False)

# 添加颜色条
sm = plt.cm.ScalarMappable(cmap=cmap, norm=norm)
sm.set_array([])
plt.colorbar(sm, label='Log Access Count')

# 添加tid作为文本标签
for page, tid_list in page_tids.items():
    tid_str = ','.join(str(tid) for tid in tid_list)
    plt.text(time_points[0], page, tid_str, fontsize=6)

# 保存图表为文件
plt.savefig('./kmeans_pattern.png')