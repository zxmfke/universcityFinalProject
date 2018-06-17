<?php
	 //echo system("H:\\vs\\getDevice\\Debug\\getDevice.exe 4");
	 $word = $_POST['word'];
	 if($word=="open")
	 system("Project_winpcap.exe 3");
	else if($word=="stop")
		system("taskkill /f /im Project_winpcap.exe");
	  // system("taskkill /f /im Project_winpcap.exe");
?>