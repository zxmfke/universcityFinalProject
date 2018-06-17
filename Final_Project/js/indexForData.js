/*********************************
函數目的:登入/登出
**********************************/

function Login(){	
	var account = $('#loginAccount').val();
	var password = $('#loginPassword').val();
	var remember = $('#remember')[0].checked;

	var checkL = /^[0-9a-zA-Z]\w{5,14}$/;

	if(account == "" || password == "" || !( checkL.test(account) ) || !( checkL.test(password) ) ){
		$('#loginError').fadeIn(300);
	}
	else{	
		$.ajax({
			//告訴程式表單要傳送到哪裡                                         
			url:"./project/php/login.php",
			
			//使用POST方法
			type : "POST",
			
			dataType:"JSON",
			//需要傳送的資料
			data: {
				id : $('#loginAccount').val(),
				pw : $('#loginPassword').val(),
				remember : remember,
			},
			
			success:function(data){                                                           
				//資料傳送成功後就會執行這個function內的程式，可以在這裡寫入要執行的程式
				if(data.result ==true){
					$('#userID').html(data.userID);
					user = data.userID;
					$('#loginError').fadeOut(300);
					$('#welcome').fadeIn(300);
					$('#login').hide();
					alert("登入成功");
					alreadyLogin = true;
					location.href="./project/mainpage.html"
				}
				else{
					$('#loginError').fadeIn(300);
				}
			}
		}); 
	}

}


/*********************************
函數目的:Register動作
**********************************/
function checkinfo(id){
	check = /^[a-z0-9]{6,15}$/;
	var account = $("#account").val();
	var password = $("#password").val();
	var password1 = $("#password1").val();
	switch(id){
		case "account":
			$("#check4").html("");
  			if(check.test(account)){
  				search_account();
  			}
  			else $("#check1").html("<img src='project/image/error.png' style='height:15px;weight:15px;''>"+"</img>");
  			
  		break;
  		case "password":
  			if(check.test(password))$("#check2").html("<img src='project/image/correct.png' style='height:15px;weight:15px;''>"+"</img>");
			else $("#check2").html("<img src='project/image/error.png' style='height:15px;weight:15px;''>"+"</img>");
  		break;
  		case "password1":
  			if(check.test(password1) && password == password1)$("#check3").html("<img src='project/image/correct.png' style='height:15px;weight:15px;''>"+"</img>");
			else $("#check3").html("<img src='project/image/error.png' style='height:15px;weight:15px;''>"+"</img>");
  		break;
	}
}

function register_account(){
	var account = $("#account").val();
	var password = $("#password").val();
	var password1 = $("#password1").val();
	check = /^[a-z0-9]{6,15}$/;
	if(check.test(account) && check.test(password) && password == password1){
		$.ajax({                                  
			url: "../project/php/register.php",
			data: {account : account,password : password,type : 'register_account'},
			type: "POST",
			success: function(data){
				// console.log(data);
				$("#register_report").html(data);
			},
			error: function() {
				alert("傳送失敗");

			}
		}); 
	}
	else alert("帳號或密碼格式錯誤！");
}

function search_account(){
	var account = $("#account").val();

	$.ajax({                                  
		url: "../project/php/register.php",
		data: {account : account,password : '',type :'search_account'},
		type: "POST",
		success: function(data){
			if(data){
				$("#check1").html("<img src='project/image/error.png' style='height:15px;weight:15px;''>"+"</img>");
				$("#check4").html(data);
			}
			else $("#check1").html("<img src='project/image/correct.png' style='height:15px;weight:15px;''>"+"</img>");
		},
		error: function() {
			alert("傳送失敗");

		}
	}); 

}