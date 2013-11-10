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

  $(document).ready(function() {
      $(".sidebar").prepend('<span id="pin"><img src="_static/unpin.png" title="click here to pin the table of contents"></span>');

      menuYloc = parseInt($(".sidebar").css("top").substring(0,$(".sidebar").css("top").indexOf("px")));
      $(".sidebar").attr("sticked", "false");

      $(window).scroll(function () {
	  if ($(".sidebar").attr("sticked") != "true") {
	    offset = menuYloc+$(document).scrollTop()+"px";  
	    $(".sidebar").animate({top:offset},{duration:500,queue:false});
	  }
	});
      $("#pin").click(event, function() {
	    if ($(".sidebar").attr("sticked") != "true") {
	      $(".sidebar").attr("sticked", "true");
		$("#pin").html('<img src="_static/pin.png" title="click here to unpin the table of contents">');
	    }
	    else {
	      $(".sidebar").attr("sticked", "false");
		$("#pin").html('<img src="_static/unpin.png" title="click here to pin the table of contents">');
	    }
 	});
    })
    })(jQuery);
