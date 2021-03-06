<?php
	require_once("connectDB.php");

	echo <<< _ListHead_
		<table class='table table-hover' id='tb1'>
				<thead>
					<tr>
						<td>編號</td>
						<td>來源IP</td>
						<td></td>
						<td>目的IP</td>
						<td>協定</td>
						<td>長度</td>
						<td>存在時間</td>
					</tr>
				</thead>
				<tbody>
_ListHead_;
	try{
		$flow_query = $db->prepare("SELECT sourceIP,destinationIP,'ARP' as type from pck_arp union SELECT sourceIP,destinationIP,case version when '4' then 'IPv4' when '3' then 'IPv6' end as type from pck_ip");
		$flow_query->execute();
		$pages = 1;
		$number = $pages;  //No.(編號)
		
		while($flow = $flow_query -> fetch()){
		$flowTlen = 0;
		$flowDetailTime = "";
			if($flow['type'] == "IPv4" || $flow['type'] == "IPv6"){
				$ipFlow_query = $db->prepare("SELECT time from pck_ip where sourceIP='$flow[sourceIP]' and destinationIP='$flow[destinationIP]'");
				$ipFlow_query->execute();
				foreach($ipFlow_query->fetchAll() as $oneFlow){
					$flowDetailTime = $flowDetailTime . $oneFlow['time'] . "-";
					$flowTlen = $flowTlen + (int)getLen($db,$oneFlow);	
				}
			}
			else if($flow['type'] == "ARP"){
				$arpFlow_query = $db->prepare("SELECT time from pck_arp where sourceIP='$flow[sourceIP]' and destinationIP='$flow[destinationIP]'");
				$arpFlow_query->execute();
				foreach($arpFlow_query->fetchAll() as $oneFlow){
					$flowDetailTime = $flowDetailTime . $oneFlow['time'] . "-";
					$flowTlen = $flowTlen + (int)getLen($db,$oneFlow);
				}
			}

			$flowDetailTime = substr($flowDetailTime,0,strlen($flowDetailTime)-1);

			echo <<< _packageListTable_
				<tr id='{$flow['sourceIP']}-{$flow[destinationIP]}'>
				<td>{$number}</td>
				<td>{$flow['sourceIP']}</td>
				<td class=putImg onclick=getFlowPck('{$flowDetailTime}','{$flow['type']}')><img src='image/contect.png' width='20' height='10'></td>
				<td>{$flow[destinationIP]}</td>
				<td>{$flow['type']}</td>
				<td>{$flowTlen}</td>
				<td>on way</td>
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

<?php
	function getLen($db,$oneFlow){
		$etherLen = $db->prepare("SELECT len from pck_ethernet where time='$oneFlow[time]'");
		$etherLen->execute();
		$l = $etherLen->fetch();
		return $l['len'];
	}
?>