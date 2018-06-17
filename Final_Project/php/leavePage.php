<?php if(!isset($_SESSION)) session_start(); ?>

<?php

	$SecurityCode = $_POST['SecurityCode'];
	$f = "../clientpackage/" . $SecurityCode . ".txt";
	unlink($f);
	echo $f;

?>