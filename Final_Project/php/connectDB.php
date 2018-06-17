<?php
	define("DB_DNS",		"mysql");
	define("DB_USERNAME",	"root");
	define("DB_PASSWORD",	"");
	define("DB_HOSTNAME",	"localhost");
	define("DB_DATABASE",	"package");

	header("Content-Type:	text/html;	charset=utf-8");

	try{
		$db = new PDO(DB_DNS . ":host=" . DB_HOSTNAME . ";dbname=" . DB_DATABASE, DB_USERNAME, DB_PASSWORD);	
	} catch(PDOException $e){
		echo "失敗";
	}

	$db->exec("SET NAME UTF-8");

?>