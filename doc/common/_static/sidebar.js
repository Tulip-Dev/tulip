// adapted from https://github.com/sphinx-doc/sphinx/blob/master/sphinx/themes/classic/static/sidebar.js_t

/*
 * sidebar.js
 * ~~~~~~~~~~
 *
 * This script makes the Sphinx sidebar collapsible.
 *
 * .sphinxsidebar contains .sphinxsidebarwrapper.  This script adds
 * in .sphixsidebar, after .sphinxsidebarwrapper, the #sidebarbutton
 * used to collapse and expand the sidebar.
 *
 * When the sidebar is collapsed the .sphinxsidebarwrapper is hidden
 * and the width of the sidebar and the margin-left of the document
 * are decreased. When the sidebar is expanded the opposite happens.
 * This script saves a per-browser/per-session cookie used to
 * remember the position of the sidebar among the pages.
 * Once the browser is closed the cookie is deleted and the position
 * reset to the default (expanded).
 *
 * :copyright: Copyright 2007-2017 by the Sphinx team, see AUTHORS.
 * :license: BSD, see LICENSE for details.
 *
 */

$(function() {

  // global elements used by the functions.
  // the 'sidebarbutton' element is defined as global after its
  // creation, in the add_sidebar_button function
  var doc = $('.document');
  var sidebar = $('.sidebar');
  var sidebarwrapper = $('.sidebarwrapper');

  // for some reason, the document has no sidebar; do not run into errors
  if (!sidebar.length) return;

  // colors used by the current theme
  var dark_color = '#88c66f';
  var light_color = '#eeeeec';

  function sidebar_is_collapsed() {
    return sidebar.is(':not(:visible)');
  }

  function toggle_sidebar() {
    if (sidebar_is_collapsed())
      expand_sidebar();
    else
      collapse_sidebar();
  }

  function collapse_sidebar() {
    sidebar.hide();
    sidebarbutton.find('span').text('»');
    sidebarbutton.attr('title', _('Expand sidebar'));
    sidebarbutton.css({
      'margin-left': '0',
      'margin-right': '15.8em'
    });
    doc.css({'width': '83em'});
    document.cookie = 'sidebar=collapsed';
  }

  function expand_sidebar() {
    sidebar.show();
    sidebarbutton.css({
        'height': doc.height()
    });
    sidebarbutton.find('span').text('«');
    sidebarbutton.attr('title', _('Collapse sidebar'));
    sidebarbutton.css({
      'margin-left': '15.8em',
      'margin-right': '0'
    });
    doc.css({'width': '64em'});
    document.cookie = 'sidebar=expanded';
  }

  function add_sidebar_button() {
    sidebarwrapper.css({
        'margin-right': '0',
        'width': '20em'
    });
    // create the button
    sidebarwrapper.append(
        '<div id="sidebarbutton"><span>&laquo;</span></div>'
    );
    var sidebarbutton = $('#sidebarbutton');

    // find the height of the viewport to center the '<<' in the page
    var viewport_height;
    if (window.innerHeight)
      viewport_height = window.innerHeight;
    else
      viewport_height = $(window).height();
    sidebarbutton.find('span').css({
        'display': 'block',
        'margin-top': (viewport_height - sidebar.position().top - 20) / 2
    });

    sidebarbutton.click(toggle_sidebar);
    sidebarbutton.attr('title', _('Collapse sidebar'));
    sidebarbutton.css({
        'color': '#000000',
        'border-right': '1px solid ' + dark_color,
        'font-size': '1.2em',
        'cursor': 'pointer',
        'height': doc.height(),
        'padding-top': '1px',
        'background-color': light_color,
        'margin-left': '15.8em'
    });

    sidebarbutton.hover(
      function () {
          $(this).css('background-color', dark_color);
      },
      function () {
          $(this).css('background-color', light_color);
      }
    );
  }

  function set_position_from_cookie() {
    if (!document.cookie)
      return;
    var items = document.cookie.split(';');
    for(var k=0; k<items.length; k++) {
      var key_val = items[k].split('=');
      var key = key_val[0].replace(/ /, "");  // strip leading spaces
      if (key == 'sidebar') {
        var value = key_val[1];
        if ((value == 'collapsed') && (!sidebar_is_collapsed()))
          collapse_sidebar();
        else if ((value == 'expanded') && (sidebar_is_collapsed()))
          expand_sidebar();
      }
    }
  }

  add_sidebar_button();
  var sidebarbutton = $('#sidebarbutton');
  set_position_from_cookie();
});
