<?php

$temperatura = $_GET['temp'];
$humedad     = $_GET['hum'];

echo "La Temperatura es: " .$temperatura."<br>La Humedad es: ".$humedad;

$usuario     = "root";
$contraseña  = "";
$servidor    = "localhost";  
$basededatos = "Idc_diplomado";

$conexion = mysqli_connect( $servidor, $usuario, "") or die("No se ha podido conectar al servidor de la base de datos");

$db = mysqli_select_db($conexion, $basededatos) or die("No se ha podido seleccionar la base de datos");

$fecha = time();
$consulta = "INSERT INTO datos (fecha , temperatura , humedad ) VALUES (".$fecha.",".$temperatura.",".$humedad.")";  

$resultado = mysqli_query( $conexion, $consulta); 

?>