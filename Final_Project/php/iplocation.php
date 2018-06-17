<?php
	// print_r($_POST);
	require_once("connectDB.php");

	$ip = $_POST['ip'];
	$type = $_POST['type'];
	$iptype = $_POST['iptype'];

	function ipv6_to_hex($ipaddress){
    $pton = @inet_pton($ipaddress);
    if (!$pton) { return false; }
    $number = '';
    foreach (unpack('C*', $pton) as $byte) {
        $number .= str_pad(decbin($byte), 8, '0', STR_PAD_LEFT);
     }
     $h='';
     $hex='';
     $k=0;
     for($i=0;$i<32;$i++){
        for($j=0;$j<4;$j++){
            $h .= $number[$k];
            $k++;
        }
        $hex .= base_convert($h,2,16);
        $h='';
     }
     return $hex;
 	}

	function get_real_ip_addr(){ 
  		if (!empty($_SERVER['HTTP_CLIENT_IP']))
			{
  				$myip=$_SERVER['HTTP_CLIENT_IP'];
			}
				else if (!empty($_SERVER['HTTP_X_FORWARDED_FOR']))
				{
  					$myip=$_SERVER['HTTP_X_FORWARDED_FOR'];
				}
		else
			{
  				$myip=$_SERVER['REMOTE_ADDR'];
			}
		return $myip;
 	}
 	
 	switch ($type) {
 		case 'search':
 			if($iptype != "ipv6"){
 				$search_ip = $ip;
 				$serach = "inet_aton(start_ip) <= inet_aton('$search_ip') AND inet_aton('$search_ip') <= inet_aton(end_ip)";
 			}
 			else {
 				$search_ip = ipv6_to_hex($ip);
 				$serach = "start_number <= '$search_ip' AND '$search_ip' <= end_number ";
 			}
 			break;
 		case 'local':
 			$search_ip = get_real_ip_addr();
 			$serach = "inet_aton(start_ip) <= inet_aton('$search_ip') AND inet_aton('$search_ip') <= inet_aton(end_ip)";
 			break;
 	}

	$query = $db->prepare("SELECT * 
							FROM pck_ipfortw 
							WHERE {$serach}");
	$query->execute();
	
	//帳號基本上只抓到一筆, fetch()即可
	$datas = $query->fetch();

	if($type == "search"){
		if($datas[4] == "" || $datas[5] == "") echo "查無此IP";
		else echo "您所查詢的IP區域為：$datas[4],$datas[5]";
	}

	if($type == "local"){
		echo "$datas[4]&nbsp;&nbsp;$datas[5]";
	}
?>