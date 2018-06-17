<?php if(!isset($_SESSION)) {session_start();}
?>
<?php
	require_once("connectDB.php");

	if( !empty($_SESSION['userAccount']) )
	{    //session有
		if( empty($_COOKIE['userAccount']) )
		{  //cookie沒  
			$query = $db -> prepare("SELECT * FROM kms_user where u_a = '{$_SESSION['userAccount']}'");
			$query -> execute();
			if( ( $data = $query -> fetch() ) && ( $_SESSION['userAccount'] = $data['u_a'] ) )
			{
				echo "{\"name\":\"".$data['name']."\",\"image\":\"".$data['u_img']."\",\"result\":\"true\"}";
			}
			else
			{
				echo "{\"result\":\"false\"}";
			}
		}
		else
		{	//cookie有 
			$accountCookie = $_COOKIE['userAccount'];
			$query = $db -> prepare("SELECT * FROM kms_user where MD5(u_a) = '{$accountCookie}'");
			$query -> execute();
			if( $data = $query -> fetch() )
			{
				$_SESSION['userAccount'] = $data['u_a'];
				setcookie("userAccount","");
				setcookie("userAccount",md5($data['account']), time()+3600*24*7,'/');
				echo "{\"name\":\"".$data['name']."\",\"image\":\"".$data['u_img']."\",\"result\":\"true\"}";
			}
			else
			{
				echo "{\"result\":\"false\"}";
			}
		} 
	}
	else
	{	//session沒
		echo "{\"result\":\"false\"}";
	}











?>