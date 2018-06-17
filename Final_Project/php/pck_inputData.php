<?php
	require_once("connectDB.php");
	$SecurityCode = $_POST['SecurityCode'];
	$user = $_POST['user'];

	// $SecurityCode = "1111111111";
	// $user = "111111";

 	$filename = "../clientpackage/" . $SecurityCode . ".txt";//定义操作文件
	$delline = 1; //要删除的行数
	$flowTable = $db -> prepare("select sourceIP,destinationIP from pck_arp union select sourceIP,destinationIP from pck_ip");
	$flowTable->execute();
	$newfp = "";
	$end = "0";
	$farray=file($filename);//读取文件数据到数组中

	if(count($farray)!=0){
		for($i=0;$i<count($farray);$i++){
			if(strcmp($i+1,$delline)==0){  //判断删除的行,strcmp是比较两个数大小的函数
				$buf = json_decode($farray[$i],true);
				switch($buf['Ethernet']['type'])
				{
					case "2048":{
						$eType = "IPv4";
						$isIP4 = "true";
						$tmp = $buf['ip4'];
						break;
					}
					case "2054":{
						$eType = "ARP";
						$isIP4 = "false";
						break;
					}
					default:{
						$eType = "IPv6";
						$isIP4 = "false";
						$tmp = $buf['ip6'];
						break;
					}
				}	

				switch($buf['tag'])
				{
					case "arp":{
						$sql = prepareQuery(null,$buf,"ARP",$eType,$user);
						$query = $db -> prepare($sql);
						$query->execute();
						$judge = exist($flowTable,$buf['arp']);
			 			printToJS(($judge=="true"?"0":$buf['time']),$buf['arp']['sip'],$buf['arp']['dip'],$buf['Ethernet']['len'],($judge=="true"?null:"ARP"),"ARP");
			 			$end ="0";
						break;
					}
					case "tcp":{
						$sql = prepareQuery($isIP4,$buf,"TCP",$eType,$user);
						$query = $db -> prepare($sql);
						$query->execute();
						$judge = exist($flowTable,$tmp);
						printToJS(($judge=="true"?"0":$buf['time']),$tmp['sip'],$tmp['dip'],$buf['Ethernet']['len'],($judge=="true"?null:$eType),"IP");
			 			break;
					}
					case "udp":{
						$sql = prepareQuery($isIP4,$buf,"UDP",$eType,$user);
						$query = $db -> prepare($sql);
						$query->execute();
						$judge = exist($flowTable,$tmp);
						printToJS(($judge=="true"?"0":$buf['time']),$tmp['sip'],$tmp['dip'],$buf['Ethernet']['len'],($judge=="true"?null:$eType),"IP");
			 			break;
					}
					case "icmp":{
						$sql = prepareQuery($isIP4,$buf,"ICMP",$eType,$user);
						$query = $db -> prepare($sql);
						$query->execute();
						$judge = exist($flowTable,$tmp);
						printToJS(($judge=="true"?"0":$buf['time']),$tmp['sip'],$tmp['dip'],$buf['Ethernet']['len'],($judge=="true"?null:$eType),"IP");
			 			break;
					}
					case "icmpv6":{
						$sql = prepareQuery($isIP4,$buf,"ICMPv6",$eType,$user);
						$query = $db -> prepare($sql);
						$query->execute();
						$judge = exist($flowTable,$tmp);
						printToJS(($judge=="true"?"0":$buf['time']),$tmp['sip'],$tmp['dip'],$buf['Ethernet']['len'],($judge=="true"?null:$eType),"IP");
						break;
					}
					default:{break;}
				}
				
				continue;
			}   
			if(trim($farray[$i])<>""){ //删除文件中的所有空行   
				$newfp = $newfp.$farray[$i];    //重新整理后的数据
			}
		}

		if(!isset($buf))$end ="1"; 
		$fp=@fopen($filename,"w");//以写的方式打开文件
		if(!$fp)echo "can not open";
		@fputs($fp,$newfp);
		@fclose($fp);
		//echo $end;
	}
	else echo "{\"error\":\"yes\"}";

?>

<?php
	function printToJS($time,$sip,$dip,$len,$type,$a){
		echo "{\"time\":\"".$time."\",\"sip\":\"".$sip."\",\"dip\":\"".$dip."\",\"protocol\":\"".$type."\",\"len\":\"".$len."\",\"type\":\"".$a."\",\"error\":\"no\"}";
	}
?>

<?php
	function exist($flowTable,$buf){
		foreach ($flowTable->fetchAll() as $td)
		{
			if($td['sourceIP']==$buf['sip'] && $td['destinationIP']==$buf['dip'])
				return "true";
		}
		return "false";
	}
?>

<?php
	function prepareQuery($isIP4,$buf,$type,$eType,$user){
		$tmp = "";
		if($isIP4=="true")$tmp = $buf['ip4'];
		else $tmp = $buf['ip6'];
		$ipSql = "";
		$ipv4Sql = "";
		$ipv6Sql = "";

		$ethernetSql = "INSERT INTO pck_ethernet VALUES( '{$user}', '{$buf['time']}', '{$buf['Ethernet']['smac']}', '{$buf['Ethernet']['dmac']}', '{$eType}', '{$buf['Ethernet']['len']}', '{$buf['Ethernet']['caplen']}' );";
		
		if($eType!="ARP"){
			$ipSql = "INSERT INTO pck_ip VALUES ('{$user}', '{$buf['time']}','{$tmp['sip']}','{$tmp['dip']}','{$tmp['version']}');";
			if($eType=="IPv4"){
				$ipv4Sql = "INSERT INTO pck_ipv4 VALUES('{$user}', '{$buf['time']}','{$type}','{$tmp['hl']}','{$tmp['off']}','{$tmp['flag']}','{$tmp['len']}','{$tmp['TTL']}');";
			}
			if($eType=="IPv6"){
				$ipv6Sql = "INSERT INTO pck_ipv6 VALUES('{$user}', '{$buf['time']}','{$type}','{$tmp['label']}','{$tmp['len']}','{$tmp['hopLimit']}');";
			}
		}

		$commonSql = $ethernetSql.$ipSql.($isIP4=="true"?$ipv4Sql:$ipv6Sql);
		
		switch($type){
			case "ARP":{
				return $ethernetSql."INSERT INTO pck_arp VALUES( '{$user}', '{$buf['time']}', '{$buf['arp']['sip']}', '{$buf['arp']['dip']}', '{$buf['arp']['opcode']}' ,'{$buf['arp']['smac']}','{$buf['arp']['dmac']}','{$buf['arp']['proType']}','{$buf['arp']['hardType']}','{$buf['arp']['pLen']}','{$buf['arp']['hLen']}');";
				break;
			}
			case "TCP":{
				return $commonSql."INSERT INTO pck_tcp VALUES('{$user}', '{$buf['time']}','{$buf['tcp']['sport']}','{$buf['tcp']['dport']}','{$buf['tcp']['seq']}','{$buf['tcp']['ack']}','{$buf['tcp']['off']}','{$buf['tcp']['flag']}','{$buf['tcp']['win']}');";
				break;
			}
			case "UDP":{
				return $commonSql."INSERT INTO pck_udp VALUES('{$user}', '{$buf['time']}','{$buf['udp']['sport']}','{$buf['udp']['dport']}','{$buf['udp']['len']}');";
				break;
			}
			case "ICMP":{
				return $commonSql."INSERT INTO pck_icmp VALUES('{$user}', '{$buf['time']}','{$buf['icmp']['type']}','{$buf['icmp']['code']}','{$buf['icmp']['id']}','{$buf['icmp']['seq']}');";
				break;
			}
			case "ICMPv6":{
				return $commonSql."INSERT INTO pck_icmp VALUES('{$user}', '{$buf['time']}','{$buf['icmp']['type']}','{$buf['icmp']['code']}');";
				break;
			}
			default:break;
		}
	}
?>
