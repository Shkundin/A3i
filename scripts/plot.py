import csv, collections
import matplotlib.pyplot as plt

def load(fname):
    D = collections.defaultdict(list)
    with open(fname, newline='') as f:
        r = csv.DictReader(f)
        for row in r:
            D[row['dataset']].append((int(row['N']), int(row['ns'])))
    for k in D:
        D[k].sort()
    return D

std = load('../results/qs_std.csv')
hyb = load('../results/qs_hybrid.csv')

for ds in std.keys():
    Ns  = [n for n,_ in std[ds]]
    Tst = [ns/1e6 for _,ns in std[ds]]
    Thy = [ns/1e6 for _,ns in hyb[ds]]

    plt.figure()
    plt.plot(Ns, Tst, marker='o', label='QS_Std')
    plt.plot(Ns, Thy, marker='o', label='QS_Hybrid')
    plt.xlabel('N')
    plt.ylabel('Время, ms (медиана R)')
    plt.title(f'Категория: {ds}')
    plt.legend()
    plt.grid(True)
    plt.tight_layout()
   
    safe = ds.replace('/','_')
    plt.savefig(f'../results/{safe}.png', dpi=150)
    # plt.show()  
