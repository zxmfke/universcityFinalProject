var stUpdateTime = 0;
var chart;
var startSt = 0;
var sportProtocol = "";
var schangetype = "TCPUDP";
var sportText = "Protocol";
var dportProtocol = "";
var dportText = "Protocol";
var dchangetype = "TCPUDP";
var pieTotal =[];
var flow = "";
var user = user;
var showip_s;
var citychart;

/****************package information****************/

/*********************************
函數名:input
函數目的:載入txt中的封包到數據庫，並把抓到的資料加到當前datatable
函數參數: 無
**********************************/

function input(user,SecurityCode){
	$.ajax({
		url:"../project/php/pck_inputData.php",
		data:{
			user: user ,
			SecurityCode: SecurityCode,
			
		},
		dataType:"JSON",
		type:"POST",
		success:function(data){
			// console.log(data);
			if(data.error!="yes"){
				if(parseInt(data.time)!=0){
					var t = $("#tb1").DataTable();
					var count = $(".sorting_1").last().html()==undefined?parseInt(0):parseInt($(".sorting_1").last().html());
					t.row.add([
						parseInt(count)+parseInt(1),
						data.sip,
						"0",
						data.dip,
						data.protocol,
						data.len,
						data.time
					]).draw(false);
					$(".sorting_1").last().parent().attr("id",data.sip+"-"+data.dip);
					$(".sorting_1").last().parent().children().eq(2).html("<img src='image/link.png'>");
					$(".sorting_1").last().parent().children().eq(2).attr("class","putImg");
					$(".sorting_1").last().parent().children().eq(2).attr("onclick","getFlowPck('"+data.sip+"-"+data.dip+"','"+data.protocol+"')");
				}
				else if(parseInt(data.time)==0){
					var trid = "#tb1 tr[id='" + data.sip +"-"+ data.dip+"']";
					var place = $(trid).children().eq(5);
					var len = parseInt(place.html()) + parseInt(data.len);
					place.html(len);
				}
			}
		}

	});
}

/*********************************
函數名:startCap
函數目的:開啟winpcap的exe檔
函數參數: 無
**********************************/
function switchPcap(word){
	$.ajax({
		url:"../project/php/pck_switchPcap.php",
		data:{word:word},
		type:"POST"
	});
}

/*********************************
函數名:getData
函數目的:取得資料庫中有的封包資訊，以流的形式展示
函數參數: 無
**********************************/
function getData(user){
	$.ajax({                                  
        	url:"../project/php/pck_packageList.php",
        	data:{user:user},
			type:"POST",
			success:function(data){
				// console.log(data);
				 $("#packageData").html(data);
				 $('#tb1').dataTable( {
				 	"scrollY":        "420px",
        			"scrollCollapse": true,
        			"paging":         false,
	         		"language": {
	         			"sSearch":"搜尋",
						"sZeroRecords": "抱歉，沒有找到",
						"sInfo": "從 _START_ 到 _END_ /共 _TOTAL_ 條流",
						"sInfoEmpty": "資料庫中無資料",
						"sInfoFiltered": "(從 _MAX_ 條資料中搜尋)"
        			}
	     		});
			}
	});
}

/*********************************
函數名:getFlowPck
函數目的:展示所選的流中的所有封包
函數參數: value --- 流所包含的所有
                    封包的到達時間
          type  --- 封包類型(IPV4,IPV6,ARP)
**********************************/
function getFlowPck(value,type){

	var ip = value.split("-");

	reGetFlowPck(value,type);

	Custombox.open({
 			target: '#modal_flowInfo',
 			effect: 'push',
 			position:'center,middle',
 		});

	$("#modal_flowInfo h4").html("封包流: "+value);
	$("#modal_flowInfo span[id=flowRefresh]").attr("onclick","reGetFlowPck('"+ip[0]+"-"+ip[1]+"','"+type+"')");
	$("#modal_flowInfo div[id=flowPckDetail]").html("可以點選一個封包，查看封包詳細資訊");
	$("#modal_flowInfo").css({width:'1500px',height:"550px"});
}

function reGetFlowPck(value,type){
	$.ajax({
		url:"../project/php/pck_ajaxFlowPck.php",
		type:"POST",
		data:{value:value,type:type,user:user},
		success:function (data) {
			// console.log(data);
			$("#flowData").html(data);
			$('#tb2').dataTable( {
			 	"scrollY":        "380px",
    			"scrollCollapse": true,
    			"paging":         false,
         		"language": {
         			"sSearch":"搜尋",
					"sZeroRecords": "抱歉，沒有找到",
					"sInfo": "從 _START_ 到 _END_ /共 _TOTAL_ 個封包",
					"sInfoEmpty": "資料庫中無資料",
					"sInfoFiltered": "(從 _MAX_ 條資料中搜尋)"
    			}
     		});

			flow = value;
			$("#tb2_info").append("<button type='button' id='st_flow' class='btn btn-link btn-sm' onclick='flowStatistics()'><img style='position: relative;top: -1px;' width='20px' src='./image/show.png'/> 點擊檢視該流統計圖表</button>");
			$("#tb2_filter").hide();
		}
	})
}


/*********************************
函數名:getDetail
函數目的:取得封包的詳細資訊
函數參數: id --- 封包到達時間
**********************************/
function getDetail(id){
	$("#flowPckDetail table").html("");
	$("#tmp table").html("");
	$.ajax({
		url:"../project/php/pck_ajaxDataPackageDetail.php",
		type:"POST",
		data:{id:id},
		dataType:"JSON",
		success:function(data){
			// console.log(data);
			var pckDetailLi = "<ul class='nav nav-tabs'><li><a data-toggle='tab' href='#'>802.3</a></li><li class='active'><a data-toggle='tab' href='#ethernet'>Ethernet Frame</a></li>";

			var detail = "";
			var etherType = data.main.type;

			var etherFrame = "<div id='ethernet' class='tab-pane fade in active'><h3>Ethernet Frame</h3><table class='table'>";
				etherFrame = etherFrame + "<tr><td>目的實體位置:"+data.main.destinationMAC+"</td><td>來源實體位置:"+data.main.sourceMAC+"</td></tr>";
		    	etherFrame = etherFrame + "<tr><td>訊框長度:"+data.main.len+"</td><td>訊框捕獲長度:"+data.main.caplen+"</td></tr>";	
		    	etherFrame = etherFrame + "<tr><td>到達時間:"+id+"</td><td>乙太網路協定:"+etherType+"</td></tr></table></div>";
		 	detail = etherFrame;

			if(etherType!="ARP")
			{
				pckDetailLi = pckDetailLi + "<li><a data-toggle='tab' href='#ipPck'>IP Package</a></li>";
				var ip = (data.ipv4!=undefined?data.ipv4:data.ipv6);
				var ipPro = (data.ipv4!=undefined?data.ipv4.protocol:data.ipv6.nextHeader);
				var isIP4 = (data.ipv4!=undefined?true:false);
				
				var ipPck = "<div id='ipPck' class='tab-pane fade'><h3>網際網路協定版本"+(isIP4?"4":"6")+"</h3><table class='table'>";
					ipPck = ipPck + "<tr><td>版本:"+ip.version+"</td><td>總長度:"+ip.length+"</td></tr>";
				
				if(isIP4){
					ipPck = ipPck + "<tr><td>來源網路位置:"+ip.sourceIP+"</td><td>目的網路位置:"+ip.destinationIP+"</td></tr>";
					ipPck = ipPck + "<tr><td>IP頭長度:"+ip.HLEN+"</td><td>IPv4協定:"+ip.protocol+"</td></tr>";
					ipPck = ipPck + "<tr><td>標誌:"+ip.flags+"</td><td>分片偏移:"+ip.fragmentOffset+"</td></tr>";
					ipPck = ipPck + "<tr><td colspan=2>存在時間:"+ip.ttl+"</td></tr></table></div>";
				}
				else {
					ipPck = ipPck + "<tr><td colspan=2>來源網路位置</td></tr>";
					ipPck = ipPck + "<tr><td colspan=2>"+ip.sourceIP+"</td></tr>";
					ipPck = ipPck + "<tr><td colspan=2>目的網路位置</td></tr>";
					ipPck = ipPck + "<tr><td colspan=2>"+ip.destinationIP+"</td></tr>";
					ipPck = ipPck + "<tr><td>label:"+ip.label+"</td><td>IPv6協定:"+ip.nextHeader+"</td></tr>";
					ipPck = ipPck + "<tr><td colspan=2>hopLimit:"+ip.hopLimit+"</td></tr></table></div>";
				}
				
				detail = detail + ipPck;

				switch(ipPro){
					case "TCP":{
						pckDetailLi = pckDetailLi + "<li><a data-toggle='tab' href='#tcpSeg'>TCP Segment</a></li>";
						var tcpSeg = "<div id='tcpSeg' class='tab-pane fade'><h3>傳輸控制協定</h3><table class='table'>";
							tcpSeg = tcpSeg + "<tr><td>来源端口:"+data.tcp.sourcePort+"</td><td>目的端口:"+data.tcp.destinationPort+"</td></tr>";
							tcpSeg = tcpSeg + "<tr><td>序列號碼:"+data.tcp.seqNumber+"</td><td>確認號碼:"+data.tcp.ackNumber+"</td></tr>";
							tcpSeg = tcpSeg + "<tr><td colspan=2>窗口大小:"+data.tcp.window+"</td></tr></table></div>";
						detail = detail + tcpSeg;
						break;
					}
					case "UDP":{
						pckDetailLi = pckDetailLi + "<li><a data-toggle='tab' href='#udpDgm'>UDP Datagram</a></li>";
						var udpDgm = "<div id='udpDgm' class='tab-pane fade'><h3>傳輸控制協定</h3><table class='table'>";
							udpDgm = udpDgm + "<tr><td>來源端口:"+data.udp.sourcePort+"</td><td>目的端口:"+data.udp.destinationPort+"</td></tr>";
							udpDgm = udpDgm + "<tr><td colspan=2>長度:"+data.udp.UDPLength+"</td></tr></table></div>";
						detail = detail + udpDgm;
						break;
					}
					case "ICMP":{
						pckDetailLi = pckDetailLi + "<li><a data-toggle='tab' href='#icmp'>ICMP Segment</a></li>";
						var icmp = "<div id='icmp' class='tab-pane fade'><h3>網際網路控制訊息通訊協定</h3><table class='table'>";
							icmp = icmp + "<tr><td>類型:"+data.icmp.type+"</td><td>編碼:"+data.icmp.code+"</td></tr>";
							icmp = icmp + "<tr><td>識別號:"+data.icmp.identifier+"</td><td>序列號碼:"+data.icmp.seqNumber+"</td></tr></table></div>";
						detail = detail + icmp;
						break;
					}
					case "ICMPv6":{
						pckDetailLi = pckDetailLi + "<li><a data-toggle='tab' href='#icmpv6'>ICMPv6 Segment</a></li>";
						var icmpv6 = "<div id='icmpv6' class='tab-pane fade'><h3>網際網路控制訊息通訊協定版本6</h3><table class='table'>";
							icmpv6 = icmpv6 + "<tr><td>類型:"+data.icmpv6.type+"</td><td>編碼:"+data.icmpv6.code+"</td></tr></table></div>";
							detail = detail + icmpv6;
						break;
					}
					default:{
						alert("error");
					}
				}
			}
			else if(etherType=="ARP"){
				pckDetailLi = pckDetailLi + "<li><a data-toggle='tab' href='#arp'>ARP</a></li>";
				var arp = "<div id='arp' class='tab-pane fade'><h3>地址解析協議</h3><table class='table'>";
					arp = arp + "<tr><td>來源網路地址:"+data.arp.sourceIP+"</td><td>目的網路地址:"+data.arp.destinationIP+"</td></tr>";
					arp = arp + "<tr><td>來源實體位置:"+data.arp.sourceMac+"</td><td>目的實體位置:"+data.arp.destinationMac+"</td></tr>";
					arp = arp + "<tr><td>協定類型:"+data.arp.proType+"</td><td>協定類型長度:"+data.arp.proLen+"</td></tr>";
					arp = arp + "<tr><td>硬體類型:"+data.arp.hardType+"</td><td>硬體類型長度:"+data.arp.hardLen+"</td></tr>";
					arp = arp + "<tr><td colspan=2>operation:"+data.arp.operation+"</td></tr></table></div>";
				detail = detail + arp;
			}

			// console.log(pckDetailLi + "</ul><div class='tab-content>"+detail+"</div>");
			$("#flowPckDetail").html(pckDetailLi + "</ul><div class='tab-content'>"+detail+"</div>");	
		}
	});
}

/****************package statistics****************/

function updateGraph(user,con_type, text, st_type, port_type){

	$.getJSON('../project/php/packageStatistics.php',{ user: user ,con_type: con_type, text: text, st_type: st_type , port_type: port_type }, function (data) {
		//console.log(data);
		if(st_type == "time" && con_type == ""){
        	if( !($.isEmptyObject(data)) ){
				chart = $('#statisticsTime').highcharts();
        		chart.series[0].setData(data,true,true,true);
			}
    	}
		if(st_type == "time" && con_type == "ip"){
	        chart = $('#statisticsSearchTime').highcharts();
	            i = 0,
	            series = chart.series[0];
	            series.addPoint(data, false, true, true);      
	        chart.redraw();
		}
		if( st_type == "protocol" && con_type == ""){
			chart = $('#statisticsProtocol').highcharts();
        	chart.series[0].setData(data,true);
		}
		// if( st_type == "protocol" && con_type == "ip"){
		// 	chart = $('#statisticsSearchProtocol').highcharts();
  //       	chart.series[0].setData(data,true);
		// }
		if( st_type == "sourcePort" && con_type != "top"){
			chart = $('#statisticsSourcePort').highcharts();
        	chart.series[0].setData(data,true);
		}
		if( st_type == "destinationPort" && con_type != "top"){
			chart = $('#statisticsDestinationPort').highcharts();
        	chart.series[0].setData(data,true);
		}
		if( st_type == "ip" && con_type == "top"){
			chart = $('#statisticsTopIP').highcharts();
        	chart.series[0].setData(data,true);

		}
		if( st_type == "port" && con_type == "top"){
			chart = $('#statisticsTopPort').highcharts();
        	chart.series[0].setData(data,true);        
		}
		if( st_type == "region"){
			chart = $('#statisticsRegion').highcharts();
        	chart.series[0].setData(data,true);   
		}
	});
}


function realtimeGraph(){
	if(refresh == true){
	    switch(_clickTab){
	    	case '#time_tab':
	    		updateGraph(user,'','','time',"");
	    		break;
	    	case '#protocol_tab':
	    		updateGraph(user,'','','protocol',"");
	    		break;
	    	case '#port_tab':
	    		updateGraph(user,sportProtocol,sportText,'sourcePort',schangetype);
	    		updateGraph(user,dportProtocol,dportText,'destinationPort',dchangetype);
	    		break;
	    	case '#topIP_tab':
	    		updateGraph(user,'top','','ip',"");
	    		break;
	    	case '#topPort_tab':
	    		updateGraph(user,'top','','port',"");
	    		break;
	    	case '#searchTime_tab':
	    		updateGraph(user,'ip',flow,'time',"");
	    		break;
	    	// case '#searchProtocol_tab':
	    	// 	updateGraph('ip',flow,'protocol',"");
	    	// 	break;
	    	default:
	    }
	}
}

//對每個封包的IP進行分析，通過highchart
function flowStatistics(){
	$("#package").animate({top:'1000px'},500);
	$("#aboutus").animate({top:'1000px'},500);
	$("#highchart").animate({top:'0px'},1000);
	$("#highchart").animate({top:'20px'},200);
	_clickTab = '#searchTime_tab'

	// $.ajax({
	// 	url:"../project/php/whichFlow.php",
	// 	type:"POST",
	// 	data:{flow:flow},
	// 	success:function (data) {
	// 		flow = data;
	// 	}
	// });

	Statistics(user,"","","time","");
	Statistics(user,"","","protocol","");
	Statistics(user,"","","sourcePort","TCPUDP");
	Statistics(user,"","","destinationPort","TCPUDP");
	Statistics(user,"top","","sourcePort","");
	Statistics(user,"top","","destinationPort","");
	Statistics(user,"top","","ip","");
	Statistics(user,"ip",flow,"time","");
	// Statistics("ip",flow,"protocol","");
	Custombox.close();
	windowMove(7);
	barMove();
	$(_clickTab).stop(false, true).fadeIn().siblings().hide();
	$('li#search_li').addClass('active').siblings('.active').removeClass('active');
}

//highchart初始化
function allStatistics(){
	windowMove(0);
	barMove();
	if( startSt == 0 ){
		Statistics(user,"","","time","");
		Statistics(user,"","","protocol","");
		Statistics(user,"","","sourcePort","TCPUDP");
		Statistics(user,"","","destinationPort","TCPUDP");
		Statistics(user,"top","","sourcePort","");
		Statistics(user,"top","","destinationPort","");
		Statistics(user,"top","","ip","");
	}
	startSt = 1;
}

function sback(){
	sportProtocol = "";
	schangetype = "TCPUDP";
	sportText = "Protocol";
	Statistics(user,"","","sourcePort","TCPUDP");
}

function dback(){
	dportProtocol = "";
	dportText = "Protocol";
	dchangetype = "TCPUDP";
	Statistics(user,"","","destinationPort","TCPUDP");
}

//畫highchart
function Statistics(user,con_type, text, st_type, port_type){
    $.getJSON('../project/php/packageStatistics.php',{ user: user, con_type: con_type, text: text, st_type: st_type , port_type: port_type}, function (data) {

    	if(st_type == "time" && con_type == ""){
    		if( !($.isEmptyObject(data)) ){
    			stUpdateTime = parseInt(data[data.length-1][0]);
    		}
    		
	        $('#statisticsTime').highcharts('StockChart', {
				chart: {
					width: $('.tab_container').width()
				}, 
	            rangeSelector : {
	                selected : 0,
	                buttons: [{
					    type: 'second',
					    count: 30,
					    text: '30sec'
					}, {
					    type: 'minute',
					    count: 1,
					    text: '1min',
					}, {
					    type: 'minute',
					    count: 10,
					    text: '10 min'
					}, {
					    type: 'ytd',
					    text: 'YTD'
					}, {
					    type: 'all',
					    text: 'All'
					}]
	            },
	            
	            scrollbar: {
	                barBackgroundColor: 'gray',
	                barBorderRadius: 7,
	                barBorderWidth: 0,
	                buttonBackgroundColor: 'gray',
	                buttonBorderWidth: 0,
	                buttonBorderRadius: 7,
	                trackBackgroundColor: 'none',
	                trackBorderWidth: 1,
	                trackBorderRadius: 8,
	                trackBorderColor: '#CCC'
            	},

	            title : {
	                text : '封包流量統計'
	            },
	            subtitle: {
		            text: '每秒抓到的封包數量'
		        },
	            tooltip: {
		            pointFormat: '<span style="color:{point.color}">\u25CF</span> {series.name}: <b>{point.y}  個</b><br/>'
		        },
	            xAxis: {
		            type: 'datetime',
		            title: {
						text: '時間'
					},
		            dateTimeLabelFormats: {
		                second: '%Y-%m-%d<br/>%H:%M:%S',
		                minute: '%Y-%m-%d<br/>%H:%M',
		                hour: '%Y-%m-%d<br/>%H:%M',
		                day: '%Y<br/>%m-%d',
		                week: '%Y<br/>%m-%d',
		                month: '%Y-%m',
		                year: '%Y'
		            }
		        },
		        yAxis: {
		        	allowDecimals: false,
		        	title: {
						text: '封包數量'
					}
		        },
			    series : [{
	                name : '封包個數',
	                data : data,
	                tooltip : {
	                    valueDecimals : 0,
	                }
				}]
	        });

		}


		if(st_type == "time" && con_type == "ip"){

	        $('#statisticsSearchTime').highcharts('StockChart', {
	        	plotOptions: {
				    series: {
				        animation: false
				    }
				},
	        	chart: {
					width: $('.tab_container').width()
				}, 
	            rangeSelector : {
	                selected : 0,
	                buttons: [{
					    type: 'second',
					    count: 30,
					    text: '30sec'
					}, {
					    type: 'minute',
					    count: 1,
					    text: '1min',
					}, {
					    type: 'minute',
					    count: 10,
					    text: '10 min'
					}, {
					    type: 'ytd',
					    text: 'YTD'
					}, {
					    type: 'all',
					    text: 'All'
					}]
	            },
	            
	            scrollbar: {
	                barBackgroundColor: 'gray',
	                barBorderRadius: 7,
	                barBorderWidth: 0,
	                buttonBackgroundColor: 'gray',
	                buttonBorderWidth: 0,
	                buttonBorderRadius: 7,
	                trackBackgroundColor: 'none',
	                trackBorderWidth: 1,
	                trackBorderRadius: 8,
	                trackBorderColor: '#CCC'
            	},

	            title : {
	                text : '封包流量統計'
	            },
	            subtitle: {
		            text: '每秒抓到 '+flow+' 網路流封包數量'
		        },
	            xAxis: {
		            type: 'datetime',
		            title: {
						text: '時間'
					},
		            dateTimeLabelFormats: {
		                second: '%Y-%m-%d<br/>%H:%M:%S',
		                minute: '%Y-%m-%d<br/>%H:%M',
		                hour: '%Y-%m-%d<br/>%H:%M',
		                day: '%Y<br/>%m-%d',
		                week: '%Y<br/>%m-%d',
		                month: '%Y-%m',
		                year: '%Y'
		            }
		        },
		        yAxis: {
		        	title: {
						text: '封包數量'
					}
		        },
			    series : [{
	                name : '封包個數',
	                data : data,
	                tooltip : {
	                    valueDecimals : 2,
	                }
				}]
	        });

		}


		if( st_type == "protocol" && con_type == ""){
			$('#statisticsProtocol').highcharts({

		        chart: {
		            plotBackgroundColor: null,
		            plotBorderWidth: null,
		            plotShadow: false,
		            type: 'pie',
		            width: $('.tab_container').width()

		        },
		        title: {
		            text: 'Protocol 統計'
		        },
		        subtitle: {
		            text: '根據所有封包的TCP,UDP,ARP,ICMP占比'
		        },
		        tooltip: {
		            pointFormat: '數量 : <b>{point.y}  個</b><br>{series.name}: <b>{point.percentage:.2f}%</b>'
		        },
		        plotOptions: {
		            pie: {
		                allowPointSelect: true,
		                cursor: 'pointer',
		                dataLabels: {
		                    enabled: true,
		                    format: '<b>{point.name}</b>: {point.percentage:.1f} %',
		                    style: {
		                        color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
		                    }
		                }
		            }
		        },
		        series: [{
		            name: "佔比",
		            colorByPoint: true,
		            data: data,
		            animation: false
		        }]
		    });
		}


		// if( st_type == "protocol" && con_type == "ip"){
		// 	$('#statisticsSearchProtocol').highcharts({

		//         chart: {
		//             plotBackgroundColor: null,
		//             plotBorderWidth: null,
		//             plotShadow: false,
		//             type: 'pie',
		//             width: $('.tab_container').width()
		//         },
		//         title: {
		//             text: 'Protocol 統計'
		//         },
		//         subtitle: {
		//             text: '根據 ' + flow + ' 網路流的所有封包的TCP,UDP,ARP,ICMP占比'
		//         },
		//         tooltip: {
		//             pointFormat: '數量 : <b>{point.y}  個</b><br>{series.name}: <b>{point.percentage:.2f}%</b>'
		//         },
		//         plotOptions: {
		//             pie: {
		//                 allowPointSelect: true,
		//                 cursor: 'pointer',
		//                 dataLabels: {
		//                     enabled: true,
		//                     format: '<b>{point.name}</b>: {point.percentage:.1f} %',
		//                     style: {
		//                         color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
		//                     }
		//                 }
		//             }
		//         },
		//         series: [{
		//             name: "佔比",
		//             colorByPoint: true,
		//             data: data,
		//             animation: false
		//         }]
		//     });
		// }



		if( st_type == "sourcePort" && con_type != "top"){
			pieTotal = [];
			for(var i=0;i<data.length;i++){
				pieTotal[i] = [data[i][0],data[i][1]];
			}

			$('#statisticsSourcePort').highcharts({
		        chart: {
		            plotBackgroundColor: null,
		            plotBorderWidth: null,
		            plotShadow: false,
		            type: 'pie',
		            width: ($('.tab_container').width()-1)/2
		        },
		        title: {
		            text: '來源 Port 統計'
		        },
		        subtitle: {
		            text: '以 '+ sportText +' 類型區分 Well Known Ports 占比'
		        },
		        tooltip: {
					formatter: function () {
						if(port_type == "TCPUDP")
							return '數量 : <b>'+ this.point.y +' 個</b><br>' + 
							'總占比 : <b>'+ data[this.point.x][2]+'%</b>'
						else
							return '數量 : <b>'+ this.point.y +' 個</b><br>' + 
							sportText +'類型占比 : <b>' + parseFloat(Math.round(this.point.percentage*100))/100 +'%</b><br>總占比 : <b>'+ data[this.point.x][2]+'%</b>';
					},
		        },
		        plotOptions: {
		            pie: {
		                allowPointSelect: true,
		                cursor: 'pointer',
		                dataLabels: {
		                    enabled: true,
		                    format: '<b>{point.name}</b>: {point.percentage:.1f} %',
		                    style: {
		                        color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
		                    }
		                }
		            }
		        },
		        series: [{
		            name: "佔比",
		            colorByPoint: true,
		            data: pieTotal,
		            animation: false,
					point:{
						events:{
							click: function (event) {
								var tmp = sportText;
								sportText = this.name;	/*說明文字用*/	
								switch(port_type){
									case 'TCPUDP':
										schangetype = 'portNumber';
										sportProtocol = this.name;
										break;
									case 'portNumber':
										schangetype = 'portNumber';
										sportText = tmp;
										break;
								}
								Statistics(user,sportProtocol,sportText,"sourcePort",schangetype);
							}
						}
					}
		        }]
		    });
		}


		if( st_type == "destinationPort" && con_type != "top"){
			pieTotal = [];
			for(var i=0;i<data.length;i++){
				pieTotal[i] = [data[i][0],data[i][1]];
			}

			$('#statisticsDestinationPort').highcharts({
		        chart: {
		            plotBackgroundColor: null,
		            plotBorderWidth: null,
		            plotShadow: false,
		            type: 'pie',
		            width: ($('.tab_container').width()-1)/2
		        },
		        title: {
		            text: '目的 Port 統計'
		        },
		        subtitle: {
		            text: '以 '+ dportText +' 類型區分 Well Known Ports 占比'
		        },
		        tooltip: {
					formatter: function () {
						if(port_type == "TCPUDP")
							return '數量 : <b>'+ this.point.y +' 個</b><br>' + 
							'總占比 : <b>'+ data[this.point.x][2]+'%</b>'
						else
							return '數量 : <b>'+ this.point.y +' 個</b><br>' + 
							dportText +'類型占比 : <b>' + parseFloat(Math.round(this.point.percentage*100))/100 +'%</b><br>總占比 : <b>'+ data[this.point.x][2]+'%</b>';
					},
		        },
		        plotOptions: {
		            pie: {
		                allowPointSelect: true,
		                cursor: 'pointer',
		                dataLabels: {
		                    enabled: true,
		                    format: '<b>{point.name}</b>: {point.percentage:.1f} %',
		                    style: {
		                        color: (Highcharts.theme && Highcharts.theme.contrastTextColor) || 'black'
		                    }
		                }
		            }
		        },
		        series: [{
		            name: "佔比",
		            colorByPoint: true,
		            data: pieTotal,
		            animation: false,
					point:{
						events:{
							click: function (event) {
								//alert(this.name + " " + this.y);
								var tmp = dportText;
								dportText = this.name;
								switch(port_type){
									case 'TCPUDP':
										dchangetype = 'portNumber';
										dportProtocol = this.name;
										break;
									case 'portNumber':
										dchangetype = 'portNumber';
										dportText = tmp;
										break;
								}
								Statistics(user,dportProtocol,dportText,"destinationPort",dchangetype);
							}
						}
					}
		        }]
		    });
		}



		if( st_type == "ip" && con_type == "top"){
			$('#statisticsTopIP').highcharts({
		        chart: {
		            type: 'column',
		            width: $('.tab_container').width()
		        },
		        title: {
		            text: 'TOP 10 網路流'
		        },
		        xAxis: {
		            type: 'category'
		        },
		        yAxis: {
					allowDecimals: false,
		            title: {
		                text: '封包個數'
		            }
		        },
		        legend: {
		            enabled: false
		        },
		        plotOptions: {
		            series: {
		                borderWidth: 0,
		                dataLabels: {
		                    enabled: true,
		                    format: '{point.y}'
		                }
		            }
		        },

		        tooltip: {
		            headerFormat: '<span style="font-size:11px">{series.name}</span><br>',
		            pointFormat: '<span style="color:{point.color}">\u25CF</span> {point.name}: <b>{point.y}  個</b><br/>'
		        },

		        series: [{
		            name: "IP",
		            colorByPoint: true,
		            data: data,
		            animation: false
		        }],
		        
		    });
		}



		if( st_type == "sourcePort" && con_type == "top"){


			$('#statisticsTopSourcePort').highcharts({
		        chart: {
		            type: 'column',
		            width: $('.tab_container').width()
		        },
		        title: {
		            text: 'TOP 10 來源 Port'
		        },
		        xAxis: {
		            type: 'category'
		        },
		        yAxis: {
		        	allowDecimals: false,
		            title: {
		                text: '封包個數'
		            }

		        },
		        legend: {
		            enabled: false
		        },
		        plotOptions: {
		            series: {
		                borderWidth: 0,
		                dataLabels: {
		                    enabled: true,
		                    format: '{point.y}'
		                }
		            }
		        },
		        tooltip: {
		            headerFormat: '<span style="font-size:11px">{series.name}</span><br>',
		            pointFormat: '<span style="color:{point.color}">\u25CF</span> {point.name}: <b>{point.y}  個</b><br/>'
		        },
		        series: [{
		            name: "Port",
		            colorByPoint: true,
		            data: data,
		            animation: false
		        }],
		        
		    });

		}



		if( st_type == "destinationPort" && con_type == "top"){


			$('#statisticsTopDestinationPort').highcharts({
		        chart: {
		            type: 'column',
		            width: $('.tab_container').width()
		        },
		        title: {
		            text: 'TOP 10 目的 Port'
		        },
		        xAxis: {
		            type: 'category'
		        },
		        yAxis: {
		        	allowDecimals: false,
		            title: {
		                text: '封包個數'
		            }

		        },
		        legend: {
		            enabled: false
		        },
		        plotOptions: {
		            series: {
		                borderWidth: 0,
		                dataLabels: {
		                    enabled: true,
		                    format: '{point.y:.1f}'
		                }
		            }
		        },
		        tooltip: {
		            headerFormat: '<span style="font-size:11px">{series.name}</span><br>',
		            pointFormat: '<span style="color:{point.color}">\u25CF</span> {point.name}: <b>{point.y}  個</b><br/>'
		        },
		        series: [{
		            name: "Port",
		            colorByPoint: true,
		            data: data,
		            animation: false
		        }],
		        
		    });

		}



    });
}

/****************package location****************/

//package location
function searchip(){
	$("#showlocation").html("");
	var ip = $("#ip").val();
	var iptype="";
	/*ipv4正規表達*/
	checkipv4 =/^((?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?))*$/;
	/*ipv6正規表達*/
	checkipv6 = /^([\da-fA-F]{1,4}:){6}((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$|^::([\da-fA-F]{1,4}:){0,4}((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$|^([\da-fA-F]{1,4}:):([\da-fA-F]{1,4}:){0,3}((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$|^([\da-fA-F]{1,4}:){2}:([\da-fA-F]{1,4}:){0,2}((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$|^([\da-fA-F]{1,4}:){3}:([\da-fA-F]{1,4}:){0,1}((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$|^([\da-fA-F]{1,4}:){4}:((25[0-5]|2[0-4]\d|[01]?\d\d?)\.){3}(25[0-5]|2[0-4]\d|[01]?\d\d?)$|^([\da-fA-F]{1,4}:){7}[\da-fA-F]{1,4}$|^:((:[\da-fA-F]{1,4}){1,6}|:)$|^[\da-fA-F]{1,4}:((:[\da-fA-F]{1,4}){1,5}|:)$|^([\da-fA-F]{1,4}:){2}((:[\da-fA-F]{1,4}){1,4}|:)$|^([\da-fA-F]{1,4}:){3}((:[\da-fA-F]{1,4}){1,3}|:)$|^([\da-fA-F]{1,4}:){4}((:[\da-fA-F]{1,4}){1,2}|:)$|^([\da-fA-F]{1,4}:){5}:([\da-fA-F]{1,4})?$|^([\da-fA-F]{1,4}:){6}:$/;
	if(checkipv4.test(ip) || checkipv6.test(ip)){
		if(checkipv6.test(ip))iptype = "ipv6";
			$.ajax({                                  
				url: "../project/php/iplocation.php",
				data: {ip : ip,type : 'search' , iptype : iptype},
				type: "POST",
				success: function(data){
				 $("#showlocation").html(data);
				},
				error: function() {
					

				}
			}); 
		}
	else alert("請輸入正確IP格式！");
}

function local(){
		$.ajax({                                  
			url: "../project/php/iplocation.php",
			data: {ip : '' ,type : 'local'},
			type: "POST",
			success: function(data){	
				if(data!="&nbsp;&nbsp;")$("#local").html(data);
				else $("#local").html("臺灣");
			},
			error: function() {
			}
		}); 
}
var city;
function showcity(_city){
	city = _city;
		switch (city) {
					case "tw-pt":
					{
						city = "屏東縣";
						$('#modal_location h4').html('屏東縣');
					}
						break;
					case "tw-tn":
					{
						city = "臺南市";
						$('#modal_location h4').html('臺南市');
					}
						break;
					case "tw-il":
					{
						city = "宜蘭縣";
						$$('#modal_location h4').html('宜蘭縣');
					}
						break;
					case "tw-ch":
					{
						city = "嘉義縣";
						$('#modal_location h4').html('嘉義縣');
					}
						break;
					case "tw-tt":
					{
						city = "臺東縣";
						$('#modal_location h4').html('臺東縣');
					}
						break;
					case "tw-ph":
					{
						city = "澎湖縣";
						$('#modal_location h4').html('澎湖縣');
					}
						break;
					case "tw-km":
					{
						city = "金門縣";
						$('#modal_location h4').html('金門縣');
					}
						break;
					case "tw-lk":
					{
						city = "連江縣";
						$('#modal_location h4').html('連江縣');
					}
						break;
					case "tw-tw":
					{
						city = "臺北市";
						$('#modal_location h4').html('臺北市');
					}
						break;
					case "tw-cs":
					{
						city = "嘉義市";
						$('#modal_location h4').html('嘉義市');
					}
						break;
					case "tw-th":
					{
						city = "臺中市";
						$('#modal_location h4').html('臺中市');
					}
						break;
					case "tw-yl":
					{
						city = "雲林縣";
						$('#modal_location h4').html('雲林縣');
					}
						break;
					case "tw-kh":
					{
						city = "高雄市";
						$('#modal_location h4').html('高雄市');
					}
						break;
					case "tw-tp":
					{
						city = "新北市";
						$('#modal_location h4').html('新北市');
					}
						break;
					case "tw-hs":
					{
						city = "新竹市";
						$('#modal_location h4').html('新竹市');
					}
						break;
					case "tw-hh":
					{
						city = "新竹縣";
						$('#modal_location h4').html('新竹縣');
					}
						break;
					case "tw-cl":
					{
						city = "基隆市";
						$('#modal_location h4').html('基隆市');
					}
						break;
					case "tw-ml":
					{
						city = "苗栗縣";
						$('#modal_location h4').html('苗栗縣');
					}
						break;
					case "tw-ty":
					{
						city = "桃園市";
						$('#modal_location h4').html('桃園市');
					}
						break;
					case "tw-cg":
					{
						city = "彰化縣";
						$('#modal_location h4').html('彰化縣');
					}
						break;
					case "tw-hl":
					{
						city = "花蓮縣";
						$('#modal_location h4').html('花蓮縣');
					}
						break;
					case "tw-nt":
					{
						city = "南投縣";
						$('#modal_location h4').html('南投縣');
					}
						break;
					
			}
}

function showallip(city){
	$.ajax({                                  
			url: "../project/php/showip.php",
			data: {city : city,type : 'showip'},
			type: "POST",
			dataType: "json",
			success: function(data){/*多行不能使用HTML，用apend*/
				var sum=0;
				var cityip_s ="<tr>"+
									"<td width='150px'align='center'>"+"IP"+"</td>"+
									"<td width='20px'>"+"</td>"+
									"<td width='150px' align='center'>"+"數量(個)"+"</td>"+
									"<td width='20px'>"+"</td>"+
									"<td width='150px' align='center'>"+"地區"+"</td>"+
									"</tr>";
				for(var i=0;i<data.length;i++){
					if(data[i][2] != ""){
						cityip_s = cityip_s +"<tr>"+
											"<td width='150px' align='center'>"+data[i][0]+"</td>"+
											"<td width='20px' align='center'>"+"-"+"</td>"+
											"<td width='150px' align='center'>"+data[i][3]+"</td>"+
											"<td width='20px' align='center'>"+"-"+"</td>"+
											"<td width='150px' align='center'>"+data[i][2]+"</td>"+
											"</tr>";
					}
					else {
						cityip_s = cityip_s +"<tr>"+
										"<td width='150px' align='center'>"+data[i][0]+"</td>"+
											"<td width='20px' align='center'>"+"-"+"</td>"+
											"<td width='150px' align='center'>"+data[i][3]+"</td>"+
											"<td width='20px' align='center'>"+"-"+"</td>"+
											"<td width='150px' align='center'>"+data[i][1]+"</td>"+
										"</tr>";
					}
					sum += parseInt(data[i][3]);
		
				}
				$('#show_ip').html(cityip_s);
				$("#count").html("總共："+sum+"個");
			},
			error: function() {
				

			}
	}); 
}

function countip(){
	$.ajax({                                  
			url: "../project/php/showip.php",
			data: {city : '' ,type : 'count'},
			type: "POST",
			dataType: "json",
			success: function(data){
				// console.log(data);
				var max=0;
				var t;
				for(var i=0;i<data.length;i++){
					if(parseInt(data[i].value) > parseInt(max))max = data[i]['value'];
				}
				if(max == 0) t = "目前資料庫內無資料";
				else t = "封包IP分佈(臺灣)";
			 	$('#ip_tw').highcharts('Map', {
				        title : {
				            text : t
				        },
				        mapNavigation: {
				            enabled: true,
				            buttonOptions: {
				                verticalAlign: 'bottom'
				            }
				        },

				        colorAxis: {
				            min: 0,
				            max: max
				        },

				        series : [{
				            data : data,
				            mapData: Highcharts.maps['countries/tw/tw-all'],
				            joinBy: 'hc-key',
				            name: 'IP個數',
				            states: {
				                hover: {
				                    color: '#BADA55'
				                }
				            },
				            dataLabels: {
				                enabled: true,
				                format: '{point.name}'
				            },
				            point: {
				                events: {
				                    click: function () {
				                        var _city= $(this).attr('hc-key'); 
				                        var countip= $(this).attr('value');

				                        if(countip!=0){
				                        	$('#show_ip').html('');
				                        	 
					                        Custombox.open({
												target: '#modal_location',
												effect: 'fadeIn',
												position:'center,middle',
												width:'600',
												open: function(){
													showcity(_city);
													showip_s = setInterval(function(){showallip(city)},1000); 
												},
												close: function(){
													clearInterval(showip_s);
												}
											});
											barMove();
	      
					                        // showallip(_city);
					                    }
				                    }
				                }
				                
				            }
				        }]
				    });
			},
			error: function() {
				
			}
	}); 
}

function updateiptw(){
	$.ajax({                                  
			url: "../project/php/showip.php",
			data: {city : '' ,type : 'count'},
			type: "POST",
			dataType: "json",
			success: function(data){
				if( !($.isEmptyObject(data)) ){
					var citychart;
					citychart = $('#ip_tw').highcharts();
        			citychart.series[0].setData(data);
					var max = 0;
					for(var i=0;i<data.length;i++){
						if(parseInt(data[i].value) > parseInt(max))max = data[i]['value'];
					}
					citychart = $('#ip_tw').highcharts();
        			colorAxis = citychart.colorAxis[0];
        			colorAxis.update({
        				max: max,
        			});
				}
			},
			error: function() {
			
			}
	}); 

}


/*********************************
函數目的:登出
**********************************/

function Logout(){
	$.ajax({                                       
			url:"./php/logout.php",
			type : "POST",
			success:function(data){ 
				location.href="../index.html";
				user = "";
				$('#welcome').hide();
				$('#login').fadeIn(300);
				alreadyLogin = false;
			}
		}); 
}

function closeShowip(){
	Custombox.close();
	clearInterval(showip_s);
}