<?php
	require_once("connectDB.php");


	$user = $_GET['user'];
	$con_type = $_GET['con_type'];
	$text = $_GET['text'];	
	$st_type = $_GET['st_type'];
	$port_type = $_GET['port_type'];


	// $user = '111111';
	// $con_type = "ip";
	// $text = "64.233.187.94-220.236.1.187";
	// $st_type = "time";
	// $port_type = "";


	

	$searchFunction = "";
	$whereFunction;
	$selectFunction;
	$portNumber = "";
	$timeArray;
	switch ($con_type) {
			case 'ip':
				$timeArray = explode("-",$text);
				$searchFunction = "WHERE account = '{$user}' ";
				$searchFunction .= "AND  sourceIP = '{$timeArray['0']}' AND destinationIP = '{$timeArray['1']}'";
				// foreach($timeArray as $time){
				// 	$searchFunction = $searchFunction . " time = '{$time}' OR ";
				// }
				// $searchFunction = substr($searchFunction, 0, -3);
				break;
			case 'top':
				$searchFunction = "Order by count DESC
								Limit 0,9";
				break;	
			default:
				if( $st_type == "time" || $st_type == "protocol")
					$searchFunction = "WHERE account = '{$user}' ";
	}

	$st = array();

	date_default_timezone_set("Asia/Taipei");
	//echo date_default_timezone_get();
	//ALL + click
	if($st_type == "time"){
		$query = $db -> prepare("SELECT T.t as time , sum(T.count) as count
								FROM (SELECT SUBSTR( time, 1, 14 ) AS t, COUNT( SUBSTR( time, 1, 14 ) ) AS count
								    FROM pck_arp
								    {$searchFunction}
								    GROUP BY t
								    UNION ALL
								    SELECT SUBSTR( time, 1, 14 ) AS t, COUNT( SUBSTR( time, 1, 14 ) ) AS count
								    FROM pck_ip
								    {$searchFunction}
								    GROUP BY t) as T
								GROUP BY time");
		$query -> execute();
		$number = 0;
		while( $data = $query -> fetch() ){
			$d=mktime(substr($data[time],8,2), substr($data[time],10,2), substr($data[time],12,2), //h-m-s
				substr($data[time],4,2), substr($data[time],6,2), substr($data[time],0,4)); // m-d-y
		
			$time = strtotime(date("Y-m-d h:i:s", $d));
			$st[$number] = array($time*1000,intval($data[count]));
			$number++;
		}
	}

	//ALL 
	if($st_type == "protocol"){
		$query = $db -> prepare("SELECT p.protocol as protocol, SUM(p.count) as count
								FROM(SELECT 'ARP' as protocol , count(time) as count
									FROM pck_arp
									{$searchFunction}
									GROUP BY protocol
									UNION ALL
									SELECT protocol , count(protocol) as count
									FROM pck_ipv4
									{$searchFunction}
									GROUP BY protocol
									UNION ALL
									SELECT nextHeader as protocol , count(nextHeader) as count
									FROM pck_ipv6
									{$searchFunction}
									GROUP BY protocol) as p
								GROUP BY protocol");
		$query -> execute();
		$number = 0;
		while( $data = $query -> fetch() ){
			$st[$number] = array($data[protocol],intval($data[count]));
			$number++;
		}
	}

	//top 10 + ALL
	if($st_type == "sourcePort"){
		switch ($port_type) {
			case 'TCPUDP':
				$selectFunction = "p.protocol as sourcePort";
				$whereFunction = "WHERE p.sourcePort < 1024 " .
								"GROUP BY protocol";
				break;
			default:
				$selectFunction = "p.sourcePort  as sourcePort";
				$portNumber = " p.sourcePort < 1024";
				$whereFunction = "WHERE p.protocol = '" . $con_type . "' && " . $portNumber .
								" GROUP BY sourcePort";
		}

		if($con_type == 'top'){
			$whereFunction = "GROUP BY sourcePort";
		}

		$query = $db -> prepare("SELECT {$selectFunction} , SUM(p.count) as count
								From(SELECT cast(sourcePort AS DECIMAL(10,0)) as sourcePort , count(sourcePort) as count, 'TCP' as protocol
								      FROM pck_tcp
								      WHERE account = '{$user}' 
								      GROUP BY sourcePort
								      UNION ALL
								      SELECT cast(sourcePort AS DECIMAL(10,0)) as sourcePort , count(sourcePort) as count, 'UDP' as protocol
								      FROM pck_udp
								      WHERE account = '{$user}' 
								      GROUP BY sourcePort) as p
								{$whereFunction}
								{$searchFunction}");
		$query -> execute();
		$number = 0;

		$sumquery = $db -> prepare("SELECT sum(p.count) as sum
									FROM(SELECT count(sourcePort) as count
										FROM pck_tcp
										WHERE account = '{$user}' 
										UNION ALL
										SELECT count(sourcePort) as count
										FROM pck_udp
										WHERE account = '{$user}' )as p");
		$sumquery -> execute();
		$sumdata = $sumquery -> fetch();

		while( $data = $query -> fetch() ){
			$st[$number] = array($data[sourcePort],intval($data[count]),number_format(floatval($data[count])/floatval($sumdata[sum]),4)*100);
			$number++;
		}
	}

	if($st_type == "destinationPort"){
		switch ($port_type) {
			case 'TCPUDP':
				$selectFunction = "p.protocol as destinationPort";
				$whereFunction = "WHERE p.destinationPort <1024 " .
								"GROUP BY protocol";
				break;
			default:
				$selectFunction = "p.destinationPort  as destinationPort";
				$portNumber = "p.destinationPort < 1024 ";
				$whereFunction = "WHERE p.protocol = '" . $con_type . "' && " . $portNumber .
								" GROUP BY destinationPort";
		}

		if($con_type == 'top')
			$whereFunction = "GROUP BY destinationPort";

		$query = $db -> prepare("SELECT {$selectFunction} , SUM(p.count) as count
								From(SELECT cast(destinationPort AS DECIMAL(10,0)) as destinationPort , count(destinationPort) as count, 'TCP' as protocol
								      FROM pck_tcp
								      WHERE account = '{$user}' 
								      GROUP BY destinationPort
								      UNION ALL
								      SELECT cast(sourcePort AS DECIMAL(10,0)) as sourcePort , count(sourcePort) as count, 'UDP' as protocol
								      FROM pck_udp
								      WHERE account = '{$user}' 
								      GROUP BY sourcePort) as p
								{$whereFunction}
								{$searchFunction}");
		$query -> execute();
		$number = 0;


		$sumquery = $db -> prepare("SELECT sum(p.count) as sum
									FROM(SELECT count(destinationPort) as count
										FROM pck_tcp
										WHERE account = '{$user}' 
										UNION ALL
										SELECT count(destinationPort) as count
										FROM pck_udp
										WHERE account = '{$user}' )as p");
		$sumquery -> execute();
		$sumdata = $sumquery -> fetch();

		while( $data = $query -> fetch() ){
			$st[$number] = array($data[destinationPort],intval($data[count]),number_format(floatval($data[count])/floatval($sumdata[sum]),4)*100);
			$number++;
		}
	}

	//top 10
	if($st_type == "ip" && $con_type == 'top'){
		$query = $db -> prepare("SELECT p.sourceIP as sourceIP, p.destinationIP as destinationIP, SUM(p.count) as count
								From(SELECT sourceIP , destinationIP, count(time) as count
									FROM pck_arp
									WHERE account = '{$user}' 
									GROUP BY sourceIP,destinationIP
									UNION ALL
									SELECT sourceIP , destinationIP, count(time) as count
									FROM pck_ip
									WHERE account = '{$user}' 
									GROUP BY sourceIP,destinationIP) as p
								GROUP BY sourceIP,destinationIP
								{$searchFunction}");
		$query -> execute();
		$number = 0;
		while( $data = $query -> fetch() ){
			$buf = $data[sourceIP] . " 到 " .$data[destinationIP];
			$st[$number] = array($buf,intval($data[count]));
			$number++;
		}
	}



	echo(json_encode($st, JSON_UNESCAPED_UNICODE));

?>