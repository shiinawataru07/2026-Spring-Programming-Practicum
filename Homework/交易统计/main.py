import pandas as pd

df = pd.read_excel("finance.xlsx", sheet_name = "Sheet1")
df = df.dropna(
    subset=["日期", "交易额"]
)
df["日期"] = pd.to_datetime(df["日期"])
df["交易额"] = df["交易额"].astype(int)
mp = {}
for index, date in df["日期"].items():
    mp[date] = mp.get(date, 0) + df.loc[index, "交易额"]

lowest = sorted(mp.items(), key = lambda x: x[1])[:3]
for obj in lowest:
    print(obj[0].strftime("%Y-%m-%d"), obj[1], obj[0].day_name())