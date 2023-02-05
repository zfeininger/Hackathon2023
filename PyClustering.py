import numpy as np
from sklearn.cluster import KMeans
import pandas as pd
​
df = pd.read_csv(r"C:\Users\jacob\OneDrive\Desktop\HartsfieldJackson_10.csv")
​
x = df[["Latitude", "Longitude"]].values
​
Q1 = np.percentile(x, 25, axis=0)
Q3 = np.percentile(x, 75, axis=0)
IQR = Q3 - Q1 # get rid of outliers
​
x = x[~((x < (Q1 - 1.5 * IQR)) | (x > (Q3 + 1.5 * IQR))).any(axis=1)]
​
kmeans = KMeans(n_clusters=10) # adjust based on size of input, elbow method
​
kmeans.fit(x)
​
labels = kmeans.predict(x) # predict!
​
c = kmeans.cluster_centers_ # coordinates to the clusters. these are the pins on the map.
​
print(c) # to view coordinates
