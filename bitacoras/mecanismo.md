# Dimensionamiento de mecanismo 
Tomamos como referencia la expresión de fuerza de salida para el mecanismo completo. La fuerza constante está dada por la siguinete ecuación
$$
    F_{out} = k L_3\frac{L_1}{L_2}
$$

Donde $L_1$ es el avance del slider principal (stepper). La sensibilidad de transofmración depende de las dimensiones totales del sinfin($L_2$) Y la distancia entre los apoyos ($L_3$)

Sea $\gamma = \frac{L1}{L2}$ y representa el porcentaje de avance del slider controlado, respecto al alcance total. 

### Requisitos 
Usamos como valores iniciales, un rango de fuerza de $(1.2 - 20) ~N$. Por tanto, buscamos que un rango de avance $(0-100)%$ esté contenido dentro de dicho rango buscado. 

### Sensibilidad del sistema 
Tomemos la ecuación 

$$
    F_{out} = \left( k \gamma \right)~ L3 = S~L_3
$$

Donde $$S$$ es la sensiblidad del a fuerza de salida a la distancia de separación de los apoyos. Buscamos minimizar esta separación, en aras de compactar el sistema. Por tanto, buscamos un alta sensibilidad que permita el rango de fuerza para un valor de separación menos.


##### Resorte 
Analizando la sensibilidad S, 

$$ \frac{\partial S}{\partial k} = \gamma $$

Por tanto si buscamos un rango de operacion cercano a 0, aumentamos la robustez del sistema a los cambios de rigidez del resorte.


Se propuso un rango de 0.1 a 0.2 de avance, por lo que se tiene 

$$
    F_{out} = k ~ \left(0.1~~~~0.2 \right)~L_3
$$

Partimos de una distancia de 7 cm como supuesto inicial. Esto tomandolo como la mitad de la longitud del transductor, para tener una proporción estética. Para el resorte, partimos de $0.5 N/mm$. 
Validamos con el alcance 

$$ F_{out} =  \left(3.5, 7.0) ~N$$









