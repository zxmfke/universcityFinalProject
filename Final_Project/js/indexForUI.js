var _clickTab = "";
var searchSourceIP = "";
var refresh = false;
var chart; 
var s;
var alreadyLogin =false;
var user = "";

$(document).ready(function(){ 

  /****************主頁 準備動作****************/
 
  $("#loginError").hide();
  $("#welcome").hide();
  $("#register").hide();

  /****************** 是否已 session (登入) *****************/
  
  $.ajax({
      type : "POST",
      dataType:"JSON",
      url:"./project/php/autoLogin.php",
      success:function(data){
          if(data.result ==true){
              $('#userID').html(data.userID);
              location.href="./project/mainpage.html"
              user = data.userID;
              $('#loginError').fadeOut(300);
              $('#welcome').fadeIn(300);
              $('#login').hide();
              alreadyLogin = true;
          }
      }
  });

  /******************登入頁面滑動*****************/
  //$("#login").animate({top:"40%"}, 200 ,'swing');
  $("#login").animate({
    top:'30%',
    opacity:'1',
  }, 1000 ,'swing');


  /******************register 按鈕動作*****************/
  $("#r").on("click",function(e){
      Custombox.open({
      target: '#register',
      effect: 'fadeIn',
      position:'[center,center]',
      width:'400'
      });
      $("#register").css({width:'400px'});

  });

   $("#register_account").click(function(){
      register_account();
  });
 
});