/*
 *  floatingmenu.js
 *  ~~~~~~~~~~~~~~~
 *
 *  tutorial code from :
 *  http://net.tutsplus.com/tutorials/html-css-techniques/creating-a-floating-html-menu-using-jquery-and-css/
 */

(function($) {
  var menuYloc = null;
  var bgUrl = null;

  $(document).ready(function(){
      menuYloc = parseInt($(".sidebar").css("top").substring(0,$(".sidebar").css("top").indexOf("px")));
      bgUrl = $(".sidebar").css("background-image").substring(0, $(".sidebar").css("background-image").indexOf("logo"));
      $(window).scroll(function () {
	  if ($(".sidebar").attr("sticked") != "true") {
	    offset = menuYloc+$(document).scrollTop()+"px";  
	    $(".sidebar").animate({top:offset},{duration:500,queue:false});
	  }
	});
      $(".sidebar" ).click(event, function() {
	  var y = event.pageY - menuYloc - $(document).scrollTop();
	  if (y > 0 && y < 181) {
	    if ($(".sidebar").attr("sticked") != "true") {
	      $(".sidebar").attr("sticked", "true");
	      $(".sidebar").css("background-image", bgUrl + "logo_sticked.png)");
	    }
	    else {
	      $(".sidebar").attr("sticked", "false");
	      $(".sidebar").css("background-image", bgUrl + "logo.png)");
	    }
          }
	});
    })
    })(jQuery);


