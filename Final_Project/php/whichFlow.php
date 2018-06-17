<?php
	require_once("connectDB.php");


	 $flowTime = $_POST['flowTime'];
	//$flowTime = '20150818151405001-1231312';
	$timeArray = explode("-",$flowTime);
	$query = $db -> prepare("SELECT T.time as time , T.sourceIP as sourceIP, T.destinationIP as destinationIP
								FROM (SELECT time, sourceIP, destinationIP
								    FROM pck_arp
								    where time = {$timeArray[0]}
								    GROUP BY time
								    UNION 
								    SELECT time, sourceIP, destinationIP
								    FROM pck_ip
								    where time = {$timeArray[0]}
								    GROUP BY time) as T
								GROUP BY time");
	$query -> execute();
	$data = $query -> fetch();
	echo $data['sourceIP'] . ' 到 ' . $data['destinationIP'];

?>