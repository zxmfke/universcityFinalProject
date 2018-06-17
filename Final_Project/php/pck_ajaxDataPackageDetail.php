<?php
	require_once("connectDB.php");

	$id = $_POST['id'];
	$json = "{";

	$sql_main = $db->prepare("select len,caplen,sourceMAC,destinationMAC,type from pck_ethernet where time='".$id."'");
	$sql_main->execute();
	$row_main = $sql_main->fetch();
	$tmp_main = "\"main\":{\"len\":\"".$row_main['len']."\",\"caplen\":\"".$row_main['caplen']."\",\"sourceMAC\":\"".$row_main['sourceMAC']."\",\"destinationMAC\":\"".$row_main['destinationMAC']."\",\"type\":\"".$row_main['type']."\"}";
	$json = $json . $tmp_main .",";

	switch($row_main['type']){
		case "IPv4":{
			$json = gotoIP("true",$db,$id,$json);
			break;
		}
		case "IPv6":{
			$json = gotoIP("false",$db,$id,$json);
			break;
		}
		case "ARP":{
			$json = gotoARP($db,$id,$json);
			break;
		}
		default:{
			echo "error".$row_main['type'];
			break;
		}		
	  }

	  echo $json."}";
	
?>

<?php
	function gotoARP($db,$id,$json){
		$sql_arp = $db->prepare("select * from pck_arp where time='".$id."'");
		$sql_arp->execute();
		$row_arp = $sql_arp->fetch();
		$tmp_arp = "\"arp\":{\"sourceIP\":\"".$row_arp['sourceIP']."\",\"destinationIP\":\"".$row_arp['destinationIP']."\",";
		$tmp_arp = $tmp_arp . "\"sourceMac\":\"".$row_arp['sourceMac']."\",\"destinationMac\":\"".$row_arp['destinationMac']."\",";
		$tmp_arp = $tmp_arp . "\"proLen\":\"".$row_arp['protocolLength']."\",\"hardLen\":\"".$row_arp['hardwareLength']."\",";
		$tmp_arp = $tmp_arp . "\"proType\":\"".$row_arp['protocolType']."\",\"hardType\":\"".$row_arp['hardwareType']."\",";
		$tmp_arp = $tmp_arp . "\"operation\":\"".$row_arp['operation']."\"}";
		$json = $json . $tmp_arp;
		return $json;
	}
?>

<?php
	function gotoIP($isIP4,$db,$id,$json){
		$sql = "select a.sourceIP,a.destinationIP,a.version";
		if($isIP4=="true"){
			$sql = $sql . ",b.HLEN,b.fragmentOffset,b.flags,b.protocol as np,b.totalLength,b.TTL";
		}
		else $sql = $sql . ",b.label,b.nextHeader as np,b.payloadLength,b.hopLimit";
		$sql = $sql . " from pck_ip as a left join " . ($isIP4=="true"?"pck_ipv4":"pck_ipv6") . " as b on a.time = b.time where a.time='" .$id. "'";

		$sql_ip = $db->prepare($sql);
		$sql_ip->execute();
		$row_ip = $sql_ip->fetch();
		$tmp_ip = ($isIP4=="true"?"\"ipv4\"":"\"ipv6\"") . ":{\"sourceIP\":\"".$row_ip['sourceIP']."\",\"destinationIP\":\"".$row_ip['destinationIP']."\"";
		$tmp_ip = $tmp_ip . ",\"version\":\"".$row_ip['version']."\"";
		if($isIP4=="true"){
			$tmp_ip = $tmp_ip . ",\"HLEN\":\"".$row_ip['HLEN']."\",\"fragmentOffset\":\"".$row_ip['fragmentOffset']."\",\"flags\":\"".$row_ip['flags']."\"";
			$tmp_ip = $tmp_ip . ",\"protocol\":\"".$row_ip['np']."\",\"length\":\"".$row_ip['totalLength']."\",\"ttl\":\"".$row_ip['TTL']."\"},";
		}
		else $tmp_ip = $tmp_ip . ",\"label\":\"".$row_ip['label']."\",\"nextHeader\":\"".$row_ip['np']."\",\"length\":\"".$row_ip['payloadLength']."\",\"hopLimit\":\"".$row_ip['hopLimit']."\"},";
		$json = $json . $tmp_ip;

		switch($row_ip['np']){
			case "TCP":{
				$json = $json . gotoIPTCP($db,$id,$json);
				break;
			}
			case "ICMP":{
				$json = $json . gotoIPICMP($db,$id,$json);
				break;
			}
			case "UDP":{
				$json = $json . gotoIPUDP($db,$id,$json);
				break;
			}
			case "ICMPv6":{
				$json = $json . gotoIPUDP($db,$id,$json);
				break;
			}
			default:break;
				
		}

		return $json;
	}
?>

<?php
	function gotoIPTCP($db,$id,$json){
		$sql_tcp = $db->prepare("select sourcePort,destinationPort,seqNumber,ackNumber,dataOffset,TCPFlags,window from pck_tcp where time='".$id."'");
		$sql_tcp->execute();
		$row_tcp = $sql_tcp->fetch();
		$tmp_tcp = "\"tcp\":{\"sourcePort\":\"".$row_tcp['sourcePort']."\",\"destinationPort\":\"".$row_tcp['destinationPort']."\"";
		$tmp_tcp = $tmp_tcp . ",\"seqNumber\":\"".$row_tcp['seqNumber']."\",\"ackNumber\":\"".$row_tcp['ackNumber']."\"";
		$tmp_tcp = $tmp_tcp . ",\"window\":\"".$row_tcp['window']."\"}";
		return $tmp_tcp;
	}
?>

<?php
	function gotoIPICMP($db,$id,$json){
		$sql_icmp = $db->prepare("select type,code,identifier,seqNumber from pck_icmp where time='".$id."'");
		$sql_icmp->execute();
		$row_icmp = $sql_icmp->fetch();
		$tmp_icmp = "\"icmp\":{\"type\":\"".$row_icmp['type']."\",\"code\":\"".$row_icmp['code']."\"";
		$tmp_icmp = $tmp_icmp . ",\"identifier\":\"".$row_icmp['identifier']."\",\"seqNumber\":\"".$row_icmp['seqNumber']."\"}";
		return $tmp_icmp;
	}
?>

<?php
	function gotoIPUDP($db,$id,$json){
		$sql_udp = $db->prepare("select sourcePort,destinationPort,UDPLength from pck_udp where time='".$id."'");
		$sql_udp->execute();
		$row_udp = $sql_udp->fetch();
		$tmp_udp = "\"udp\":{\"sourcePort\":\"".$row_udp['sourcePort']."\",\"destinationPort\":\"".$row_udp['destinationPort']."\"";
		$tmp_udp = $tmp_udp . ",\"UDPLength\":\"".$row_udp['UDPLength']."\"}";
		return $tmp_udp;
	}
?>

<?php
	function gotoIPICMPv6($db,$id,$json){
		$sql_icmpv6 = $db->prepare("select type,code from pck_icmpv6 where time='".$id."'");
		$sql_icmpv6->execute();
		$row_icmpv6 = $sql_icmpv6->fetch();
		$tmp_icmpv6 = "\"icmpv6\":{\"type\":\"".$row_icmpv6['type']."\",\"code\":\"".$row_icmpv6['code']."\"}";
		return $tmp_icmpv6;
	}
?>