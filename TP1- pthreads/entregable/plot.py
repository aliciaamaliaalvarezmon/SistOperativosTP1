import matplotlib.pyplot as plt
#import seaborn as sns
#sns.set(style="darkgrid")

def datos(nombre_archivo):
    valores = open(nombre_archivo, "r") 
    valores = valores.read()
    valores = [float(x) for x in valores.split()]
    return valores

def bloques(valores,intervalo):
    res = []
    valor = 0
    for elem in range(0,len(valores)):
        if elem % intervalo == 0 and elem != 0:
            res.append(valor/intervalo)
            valor = 0
        valor += valores[elem]
    res.append(valor/intervalo)
    return res

nada = [None]

concuSI = datos("concuSI.txt")
concuNO = datos("concuNO.txt")

#print(sarasa)

plt.semilogy()
plt.plot(concuSI, label="Concurrente")
plt.plot(concuNO, label="No Concurrente")
plt.legend(bbox_to_anchor=(1.05, 1), loc=2, borderaxespad=0.)
#plt.legend( ('Relaciones'), loc = 'upper left')
plt.grid(True)
plt.grid(color = '0.5', linestyle = '--', linewidth = 1)
plt.axis('tight')
#plt.axis([0, 100, 0, 20])
plt.xlabel(r"Cantidad de Archivos", fontsize = 15, color = 'black')
plt.ylabel(r"Tiempo en segundos", fontsize = 15, color = 'black')
plt.title(' Maximum Concurrente vs No Concurrente',fontsize = 15, color = 'black', verticalalignment = 'baseline', horizontalalignment = 'center')
<<<<<<< HEAD
plt.show()
=======
plt.show()
>>>>>>> refs/remotes/origin/master
