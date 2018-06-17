<?php if(!isset($_SESSION)) session_start(); ?>

<?php
	require_once("connectDB.php");

	$id = $_POST['id'];
	$pw = md5($_POST['pw']);
	$remember = $_POST['remember'];

	// $id = '111111';
	// $pw = md5('111111');
	// $remember = true;

	//搜尋資料庫資料
	$query = $db -> prepare("SELECT * FROM pck_user where account = '{$id}' and password = '{$pw}'");
	$query -> execute();
	if( $data = $query -> fetch() ){

		if($remember == "true"){
			setcookie("userAccount","",time()-1,'/');
			setcookie("userAccount",md5($id), time()+3600*24*7,'/');
		}
		$_SESSION['userAccount'] = $id;
		echo json_encode(array(
			'userID' => $id,
			'result' => true,
		), JSON_UNESCAPED_UNICODE);
	}
	else{
		echo json_encode(array(
			'result' => false,
		), JSON_UNESCAPED_UNICODE);
	}
?>