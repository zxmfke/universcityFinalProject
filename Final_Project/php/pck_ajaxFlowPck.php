<?php
	require_once("connectDB.php");

	$value = $_POST['value'];
	$type = $_POST['type'];
	$ipArray = explode("-",$value);
	$asip = $ipArray[0];
	$adip = $ipArray[1];
	$number = 1;
	echo <<< _ListHead_
		<table class='table table-hover' id='tb2'>
				<thead>
					<tr>
						<td>編號</td>
						<td>來源IP</td>
						<td>目的IP</td>
						<td>協定</td>
						<td>長度</td>
						<td>到達時間</td>
					</tr>
				</thead>
				<tbody>
_ListHead_;
	
		$table = "";
		$protocol = "";
		if($type=="IPv4"){
			$table = "pck_ip";
			$ipTable = "pck_ipv4";
			$protocol = "i.protocol";
		}
		else if($type=="IPv6"){
			$table = "pck_ip";
			$ipTable = "pck_ipv6";
			$protocol = "i.nextHeader";
		}
		else if($type=="ARP"){
			$table = "pck_arp";
			$protocol = "e.type";
		}

		$sql = "SELECT a.time,a.sourceIP,a.destinationIP,e.len,{$protocol} as et from {$table} as a left join pck_ethernet as e on a.time=e.time";
		$sql = $sql.($type=="ARP"?" ":" left join {$ipTable} as i on a.time=i.time");
		$sql = $sql." where a.sourceIP='{$asip}' and a.destinationIP='{$adip}'";
		
		try{
			$query = $db->prepare($sql);
		
			$query->execute();

			while($row = $query->fetch()){

			$time = substr($row['time'],0,4)."/".substr($row['time'],4,2)."/".substr($row['time'],6,2)." ".substr($row['time'],8,2).":".substr($row['time'],10,2).":".substr($row['time'],12,2).":".substr($row['time'],14,3);
echo <<< _packageListTable_
				<tr onclick=getDetail('{$row['time']}')>
				<td>{$number}</td>
				<td>{$row['sourceIP']}</td>
				<td>{$row['destinationIP']}</td>
				<td>{$row['et']}</td>
				<td>{$row['len']}</td>
				<td>{$time}</td>
				</tr>
_packageListTable_;
			$number++;
			}
		}	
		catch(Exception $e){
			echo $e;
		}

	echo <<< _TableEnd_
				</tbody>
			</table>
_TableEnd_;
?>