<?php
	require_once("connectDB.php");

	$account = $_POST['account'];
	$password = $_POST['password'];
	$type = $_POST['type'];

	$s_password = md5($password);

	switch ($type) {
		case 'register_account':
			$reg_query = $db->prepare("INSERT INTO pck_user VALUES('$account', '$s_password');");
	
			if($reg_query->execute()){
				echo "<script>alert('註冊成功！');</script>";
				echo "<script>location.href='index.html'</script>";
			}
		break;
		
		case 'search_account':
			$sea_query = $db->prepare("SELECT * FROM pck_user WHERE account = '$account'");
			$sea_query->execute();
			if($sea_query->fetch())echo "此帳號已存在，請重新輸入！";
		break;
	}
	
?>