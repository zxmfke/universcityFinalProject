<?php if(!isset($_SESSION)) session_start(); ?>
<?php
	require_once("connectDB.php");

	unset($_SESSION['userAccount']);
	setcookie("userAccount","",time()-1,'/');
?>