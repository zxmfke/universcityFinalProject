var _clickTab = "";
var searchSourceIP = "";
var refresh = false;
var chart; 
var s;
var alreadyLogin =false;
var user = "";

$(document).ready(function(){ 

  /****************主頁 準備動作****************/

	// $("#container").css('height',($(window).height()+'px'));
	// $("#container").css('width',($(window).width()+'px'));

  $("#modal_pDetail").hide();
  $("#modal_condiction").hide();
  $("#modal_aboutUs").hide();
  $("#modal_location").hide();
  $("#modal_flowInfo").hide();
  $("#modal_set").hide();
  $(".mask").hide();
  local();



  $("#title").animate({
    top:'25%',
    opacity:'1',
  }, 1000 ,'swing');



  // /****************關閉頁面動作****************/
  // $(window).on('beforeunload', function() {
  //   var SecurityCode = $("#SecurityCode")[0].value;
  //   $.ajax({                                  
  //       url:"../project/php/leavePage.php",
  //       data:{SecurityCode:SecurityCode},
  //       type:"POST",
  //       success:function(data){
  //         return "You should keep this page open.";
  //       }
  //   });
  // });

  /****************流量分析 準備動作****************/

  windowMove(0);
  $('#port_tab button').hide();

  /*********************側邊欄*********************/

	// $("#mwt_tab").css('height',($(window).height()+'px'));
	var w = $("#mwt_slider_content").width();
		// $('#mwt_slider_content').css('height', ($(window).height() ) + 'px' );  /*選單離網頁下方的距離*/
	
		$("#mwt_tab").click(function(){
      if($("#mwt_mwt_slider_scroll").css('left') == '-'+w+'px')
      {
          $("#item").attr("src","image/close.png");
				  $("#mwt_mwt_slider_scroll").animate({ left:'0px' }, 200 ,'swing');
				  $("#container").animate({ left:'200px' }, 200 ,'swing');
			}
      else if($("#mwt_mwt_slider_scroll").css('left') == '0px')
      {
          $("#item").attr("src","image/item.png");
          $("#mwt_mwt_slider_scroll").animate( { left:'-'+w+'px' }, 200 ,'swing');
          $("#container").animate({ left:'0px' }, 200 ,'swing');
      }
  });

	$('div .menu img').hoverpulse({
       size: 10,  // number of pixels to pulse element (in each direction)
       speed: 200 // speed of the animation
  });

  $("#pckinfo").click(function(){
      if( alreadyLogin ){
        $("#main").hide();
        $("#highchart").animate({top:'1000px'},500);
        $("#ipLocation").animate({top:'1000px'},500);
        $("#package").animate({top:'0px'},500);
        $("#package").animate({top:'20px'},200);
        getData(user);
        barMove();
      }else{
        alert("請先進行登入動作!")
      }
  });

  $("#statistics").on("click",function(e){
      if( alreadyLogin ){
        allStatistics();
        _clickTab = "#time_tab";
        $("#main").hide();
        $("#package").animate({top:'1000px'},500);
        $("#ipLocation").animate({top:'1000px'},500);
        $("#highchart").animate({top:'0px'},500);
        $("#highchart").animate({top:'20px'},200);
        barMove();
      }else{
        alert("請先進行登入動作!")
      }
  });
  	
  $("#location").click(function(){
      if( alreadyLogin ){
        countip();
        $("#main").hide();
        $("#package").animate({top:'1000px'},500);
        $("#highchart").animate({top:'1000px'},500);
        $("#ipLocation").animate({top:'0px'},500);
        $("#ipLocation").animate({top:'20px'},200);
        barMove();
      }else{
        alert("請先進行登入動作!")
      }
  });

  $("#us").on("click",function(e){
      Custombox.open({
      target: '#modal_aboutUs',
      effect: 'fadeIn',
      position:'center,middle',
      width:'600'
      });
      barMove();
  });

	$("#logo").click(function (){
		location.href=location.href;
	});

  $("#set").click(function (){
    Custombox.open({
        target: '#modal_set',
        effect: 'slide',
        position: 'center,middle',
    });
    $("#modal_set").css({width:'300px'});
  });

  $("table[id=about_us] img").on("click",function(e){
  	  imageChange($(this));
  });

  /****************** 是否已 session (登入) *****************/
  
  $.ajax({
      type : "POST",
      dataType:"JSON",
      url:"./php/autoLogin.php",
      success:function(data){
          if(data.result ==true){
              $('#userID').html(data.userID);
              user = data.userID;
              $('#loginError').fadeOut(300);
              $('#welcome').fadeIn(300);
              $('#login').hide();
              alreadyLogin = true;
          }
          else{
              location.href="../index.html"
          }
      }
  });

 
  /******************set 按鈕動作*****************/

  $("#switch_packageInfo").on("click",function(){
    var checkL = /^[0-9]{10}$/;
    if($(this).is(":checked")){
      // if ( checkL.test($("#SecurityCode")[0].value) ){
        s=setInterval(function() {
                   input(user,$("#SecurityCode")[0].value);
                  },1000);
        alert("封包展示 動態更新");
      // }
      // else{
      //   alert("驗證碼錯誤");
      // }
    }
    else {
      clearInterval(s);
    }
  });

  $("#switch_statsitic").on("click",function(){
    if($(this).is(":checked")){
      refresh = true;
      s=setInterval('realtimeGraph()',3500);
      alert("流量統計 動態更新");
      }
    else {
      refresh = false;
      clearInterval(s);
    }
  });

  $("#switch_location").on("click",function(){
    if($(this).is(":checked")){
      s=setInterval('updateiptw()',1000);
      alert("位置辨識 動態更新");
      }
    else clearInterval(s);
  });
  
  /******************流量統計 tab動作*****************/

  setInterval('realtimeGraph()',1000);

  $('#top_tab').hide();
  $('#search_tab').hide();

  $("#top_tab").click(function(){
    $('#top_tab').fadeIn();
  });
  $("#top_tab").mouseleave(function(){
    $('#top_tab').hide();
  });
  $("#search_tab").click(function(){
    $('#search_tab').fadeIn();
  });
  $("#search_tab").mouseleave(function(){
    $('#search_tab').hide();
  });

  Highcharts.setOptions({
        global: {
          useUTC: false
        },
        credits : {
            enabled : false//去掉右下角的标志
        },
        exporting:{
            enabled:false//去掉截图
        }
    });

  /******************封面時鐘*****************/

  setInterval(function() {
    function r(el, deg) {
      el.setAttribute('transform', 'rotate('+ deg +' 500 500)')
    }
    var d = new Date()
    r(sec, 6*d.getSeconds())
    r(min, 6*d.getMinutes())
    r(hour, 30*(d.getHours()%12) + d.getMinutes()/2)
  }, 1000);
 
});

//流量統計頁面的tab標籤的移動
function windowMove(number){
  var _showTab = number;
  $('.abgne_tab').each(function(){
    var $tab = $(this);
    var $defaultLi = $('ul.tabs li', $tab).eq(_showTab).addClass('active');
    $($defaultLi.find('a').attr('href')).siblings().hide();
    $('ul.tabs > li', $tab).hover(function() {
          var $this = $(this);
          _clickTab = $this.find('a').attr('href');
          if(_clickTab != "#top_tab" && _clickTab != "#search_tab"  ){
            $this.addClass('active').siblings('.active').removeClass('active');
            $(_clickTab).stop(false, true).fadeIn().siblings().hide();
            if($("#top_tab").is(':visible'))
              $("#top_tab").hide();
            if($("#search_tab").is(':visible'))
              $("#search_tab").hide();
          }
          else{
            $(_clickTab).stop(false, true).fadeIn();
            if( _clickTab == "#top_tab")
                $("#search_tab").hide();
              else
                $("#top_tab").hide();
          }
          if( startSt == 1 && _clickTab =="#port_tab")
            $('#port_tab button').show();

      return false;
    }).find('a').focus(function(){
      this.blur();
    });

    $('ul.tabs ul > li', $tab).click(function() {
            var $this = $(this);
            _clickTab = $this.find('a').attr('href');
            if(_clickTab == "#topIP_tab" || _clickTab == "#topPort_tab")
              $('li#top_li').addClass('active').siblings('.active').removeClass('active');
            if(_clickTab == "#searchTime_tab" || _clickTab == "#searchProtocol_tab")
              $('li#search_li').addClass('active').siblings('.active').removeClass('active');
            $(_clickTab).stop(false, true).fadeIn().siblings().hide();
            return false;
          }).find('a').focus(function(){
      this.blur();
    });  
  });
}

//側邊欄的移動
function barMove(){
  $("#container").animate({ left:'0px' }, 200 ,'swing');
  var w = $("#mwt_slider_content").width();
  if ($("#mwt_mwt_slider_scroll").css('left') == '0px')
            {
                $("#item").attr("src","image/item.png");
                $("#mwt_mwt_slider_scroll").animate( { left:'-'+w+'px' }, 200 ,'swing');
            }
}

//show和hide的圖片轉換
function imageChange(a){
      var trHead = a.parent().parent().next();
      a.attr("src",(trHead.is(":visible")==true)?"image/show.png":"image/hide.png");
      trHead.slideToggle("fast");
}