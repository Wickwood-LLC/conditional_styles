About Conditional Stylesheets

  Internet Explorer implements a proprietary technology called Conditional
  Comments. While web developers frown upon technologies that aren't
  cross-browser supported, many CSS developers have found Conditional Comments
  very useful. They can fix the broken rendering of CSS in IE by placing IE-only
  CSS inside conditional comments.

  This module allows themes to easily add conditional stylesheets to the the
  theme's .info file.


Theme Users

  You only need to enable this module if a theme requires that you use it. Once
  it is enabled, the module automatically performs all of its work for any
  theme requiring it. You don't need to configure anything.


Theme Developers

  Before this module was available the only way to have IE conditional
  stylesheets was to hard-code them into your page.tpl.php. This module allows
  you to add "conditional-stylesheets" lines to your theme's .info file.

  The syntax for that is:
    conditional-stylesheets[CONDITIONAL][MEDIA][] = stylesheet.css

    where
      CONDITION can be any of the conditions specified in:
        http://msdn.microsoft.com/en-us/library/ms537512.aspx
      MEDIA can be any of the normal CSS media keywords.

  For example, to add a stylesheet that only targets IE 6 and below, use:
    conditional-stylesheets[if lt IE 7][all][] = ie6-and-below.css

  And to add a print stylesheet for IE8 only, use:
    conditional-stylesheets[if IE 8][print][] = ie8.css
