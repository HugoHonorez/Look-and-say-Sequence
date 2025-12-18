import subprocess
import matplotlib.pyplot as plt
import pandas as pd
import dataframe_image as dfi
import time
import os
import sys

C_PROGRAM = "./C/main"
PY_SCRIPT = "./Python/code.py"
MAX_ITERATIONS = 70
START_VALUE = "1"

def get_perf(program_cmd, input_str):
    cmd = ["/usr/bin/time", "-f", "%M"] + program_cmd if os.path.exists("/usr/bin/time") else program_cmd
    
    start = time.time()
    try:
        res = subprocess.run(cmd + ["--output", "temp.txt"], input=input_str, text=True, capture_output=True)
        duration = time.time() - start
        
        if res.returncode != 0: return None, None, 0
        
        try: ram_mb = int(res.stderr.strip().split('\n')[-1]) / 1024.0
        except: ram_mb = 0
        
        size = 0
        if os.path.exists("temp.txt"):
            size = os.path.getsize("temp.txt") - 1
            
        return duration, ram_mb, size
    except:
        return None, None, 0

if not os.path.exists(C_PROGRAM):
    print(f"ERREUR: {C_PROGRAM} introuvable.")
    sys.exit(1)

print(f"Benchmark - {MAX_ITERATIONS} itérations...")

x_axis = []
t_c_list, r_c_list = [], []
t_py_list, r_py_list = [], []

table_data = []

for i in range(1, MAX_ITERATIONS + 1):
    print(f"Mesure {i}/{MAX_ITERATIONS}", end="\r")
    inp = f"{START_VALUE} {i}"
    
    tc, rc, size = get_perf([C_PROGRAM], inp)
    
    if len(t_py_list) > 0 and t_py_list[-1] is not None and t_py_list[-1] > 3600.0:
        tpy, rpy = None, None
    else:
        tpy, rpy, _ = get_perf([sys.executable, PY_SCRIPT], inp)

    speedup = tpy / tc if (tc and tpy and tc > 0) else None
    ram_ratio = rpy / rc if (rc and rpy and rc > 0) else None
    th_c = size / tc if (tc and tc > 0) else 0
    th_py = size / tpy if (tpy and tpy > 0) else 0

    if tc is not None:
        x_axis.append(i)
        t_c_list.append(tc); r_c_list.append(rc)
        t_py_list.append(tpy); r_py_list.append(rpy)
    
    table_data.append({
        "Iter": i,
        "Longueur": size,
        "Temps C (s)": tc,
        "Temps Py (s)": tpy,
        "Speedup (x)": speedup,
        "Débit C (char/s)": th_c,
        "Débit Py (char/s)": th_py,
        "RAM C (MB)": rc,
        "RAM Py (MB)": rpy,
        "Ratio RAM (x)": ram_ratio
    })

if os.path.exists("temp.txt"): os.remove("temp.txt")

print("\nGénération des fichiers images")

plt.figure(figsize=(10, 6))
plt.plot(x_axis, t_c_list, label='C', linewidth=2)
valid_py = [(x, y) for x, y in zip(x_axis, t_py_list) if y is not None]
if valid_py: plt.plot([v[0] for v in valid_py], [v[1] for v in valid_py], label='Python', linewidth=2)
plt.title("Temps d'exécution"); plt.xlabel("Itérations"); plt.ylabel("Temps (s)"); plt.legend(); plt.grid(True, linestyle='--')
plt.savefig("benchmark_graph_temps.png", dpi=100)

plt.figure(figsize=(10, 6))
plt.plot(x_axis, r_c_list, label='C', linewidth=2)
valid_ram = [(x, y) for x, y in zip(x_axis, r_py_list) if y is not None]
if valid_ram: plt.plot([v[0] for v in valid_ram], [v[1] for v in valid_ram], label='Python', linewidth=2)
plt.title("Consommation RAM"); plt.xlabel("Itérations"); plt.ylabel("RAM (MB)"); plt.legend(); plt.grid(True, linestyle='--')
plt.savefig("benchmark_graph_ram.png", dpi=100)

df = pd.DataFrame(table_data)

indices = [i for i in range(len(df)) if (i < 10) or ((i+1) % 5 == 0) or (i >= len(df) - 10)]
df_view = df.iloc[indices].copy()

styler = (df_view.style
    .format({
        "Longueur": "{:,}",
        "Temps C (s)": "{:.4f}",
        "Temps Py (s)": "{:.4f}",
        "Speedup (x)": "{:.1f}",
        "Débit C (char/s)": "{:,.0f}",
        "Débit Py (char/s)": "{:,.0f}",
        "RAM C (MB)": "{:.1f}",
        "RAM Py (MB)": "{:.1f}",
        "Ratio RAM (x)": "{:.1f}"
    }, na_rep="-")
    
    .set_properties(subset=["Speedup (x)", "Ratio RAM (x)"], **{'background-color': '#d9d9d9', 'font-weight': 'bold'})
    
    .hide(axis="index")
    .set_caption("Benchmark C vs Python")
    .set_table_styles([
        {'selector': 'th', 'props': [
            ('background-color', '#404040'), 
            ('color', 'white'), 
            ('font-weight', 'bold'), 
            ('text-align', 'center'),
            ('vertical-align', 'middle')
        ]},
        {'selector': 'td', 'props': [
            ('padding', '6px'), 
            ('text-align', 'right'), 
            ('border-bottom', '1px solid #ddd')
        ]}
    ])
)

dfi.export(styler, "benchmark_tableau.png", table_conversion="matplotlib")
print("Terminé")