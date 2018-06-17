<?php if(!isset($_SESSION)) session_start(); ?>
<?php
	require_once("connectDB.php");

	$city = $_POST['city'];
	$type = $_POST['type'];

	$user = $_SESSION['userAccount'];

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

 	$myip = get_real_ip_addr();

	switch ($type) {
		case 'count':
			$countcity = ",count(sourceIP) as countcity";
			$ipcity = "RIGHT JOIN pck_ipfortw
						ON sourceIP >= inet_aton(start_ip) && sourceIP <= inet_aton(end_ip)
						group by city";

		break;
		case 'showip':
			$countcity = "";
			$ipcity = ", pck_ipfortw
						where sourceIP >= inet_aton(start_ip) && sourceIP <= inet_aton(end_ip) && city = '$city'";

		break;
	}
	
		$queryip = $db->prepare("SELECT inet_ntoa(sourceIP) ,city ,region ,count{$countcity}
								FROM(SELECT inet_aton(sourceIP) as sourceIP, count(time) as count from pck_ip where destinationIP = '$myip' AND account = '$user'
     								group by sourceIP
      								union all 
     								SELECT inet_aton(sourceIP) as sourceIP, count(time) as count from pck_arp where destinationIP = '$myip' AND account = '$user'
     								group by sourceIP
      								) as ip {$ipcity}");
		$queryip -> execute();

		$arraycount = array();

		$arrayshow = array();

		if( $type == "count"){
			$arraycount[0] = array("hc-key"  => "tw-lk","value" => 0);
			$n=1;
			while($dataip = $queryip->fetch()){
				switch ($dataip[1]) {
					case "屏東縣":
						$dataip[1] = "tw-pt";
						break;
					case "臺南市":
						$dataip[1] = "tw-tn";
						break;
					case "宜蘭縣":
						$dataip[1] = "tw-il";
						break;
					case "嘉義縣":
						$dataip[1] = "tw-ch";
						break;
					case "臺東縣":
						$dataip[1] = "tw-tt";
						break;
					case "澎湖縣":
						$dataip[1] = "tw-ph";
						break;
					case "金門縣":
						$dataip[1] = "tw-km";
						break;
					case "連江縣":
						$dataip[1] = "tw-lk";
						break;
					case "臺北市":
						$dataip[1] = "tw-tw";
						break;
					case "嘉義市":
						$dataip[1] = "tw-cs";
						break;
					case "臺中市":
						$dataip[1] = "tw-th";
						break;
					case "雲林縣":
						$dataip[1] = "tw-yl";
						break;
					case "高雄市":
						$dataip[1] = "tw-kh";
						break;
					case "新北市":
						$dataip[1] = "tw-tp";
						break;
					case "新竹市":
						$dataip[1] = "tw-hs";
						break;
					case "新竹縣":
						$dataip[1] = "tw-hh";
						break;
					case "基隆市":
						$dataip[1] = "tw-cl";
						break;
					case "苗栗縣":
						$dataip[1] = "tw-ml";
						break;
					case "桃園市":
						$dataip[1] = "tw-ty";
						break;
					case "彰化縣":
						$dataip[1] = "tw-cg";
						break;
					case "花蓮縣":
						$dataip[1] = "tw-hl";
						break;
					case "南投縣":
						$dataip[1] = "tw-nt";
						break;
					
				}
				$arraycount[$n] = array("hc-key"  => $dataip[1],"value" => $dataip[4]
					);
				$n++;
		}
			echo json_encode($arraycount);
		}
		
		if( $type == "showip")
			{
				$i=0;
			while($dataip = $queryip->fetch()){
				$arrayshow[$i] = array($dataip[0], $dataip[1], $dataip[2], $dataip[3]); /*二維陣列*/
				$i++;
			}
			echo json_encode($arrayshow);

		}
?>