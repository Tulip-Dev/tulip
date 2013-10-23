/*
 *  floatingmenu.js
 *  ~~~~~~~~~~~~~~~
 *
 *  tutorial code from :
 *  http://net.tutsplus.com/tutorials/html-css-techniques/creating-a-floating-html-menu-using-jquery-and-css/
 */

(function($) {
  var menuYloc = null;

  $(document).ready(function(){
    menuYloc = parseInt($(".sidebar").css("top").substring(0,$(".sidebar").css("top").indexOf("px")));
    $(window).scroll(function () {
      offset = menuYloc+$(document).scrollTop()+"px";  
      $(".sidebar").animate({top:offset},{duration:500,queue:false});
    });
  })
})(jQuery);


