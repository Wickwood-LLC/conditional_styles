<?php
// $Id$

/**
 * @file
 * Allows themes to add conditional stylesheets.
 *
 * @see http://msdn.microsoft.com/en-us/library/ms537512.aspx
 */

/**
 * Return paths for the theme and its base themes.
 *
 * @param $theme
 *   The name of the theme.
 * @return
 *   An array of all the theme paths.
 */
function conditional_styles_paths_to_basetheme($theme) {
  static $theme_paths;
  if (empty($theme_paths[$theme])) {
    // Find all the base themes and their paths.
    $curr = $theme;
    $themes = list_themes();
    $theme_paths[$theme] = array();
    do {
      // Save the theme name and the path.
      $theme_paths[$theme][$curr] = dirname($themes[$curr]->filename);
    } while (!empty($themes[$theme][$curr]->base_theme) && $curr = $themes[$theme][$curr]->base_theme);
    // Sort into base-theme-to-active-theme order.
    $theme_paths[$theme] = array_reverse($theme_paths[$theme], TRUE);
   }
  return $theme_paths[$theme];
}

/**
 * When the theme registry is rebuilt, we also build the conditional stylesheets.
 */
function _conditional_styles_theme($existing, $type, $theme, $path) {
  // Process the conditional stylesheets.
  $themes = list_themes();
  $paths = conditional_styles_paths_to_basetheme();

  // Grab all the conditional stylesheets.
  $stylesheets = array();
  // Start with the base theme and travel up the chain to the active theme.
  foreach ($paths AS $theme_name => $path) {
    // Look at the conditional-stylesheets defined in the theme's .info file.
    if (!empty($themes[$theme_name]->info['conditional-stylesheets'])) {
      foreach ($themes[$theme_name]->info['conditional-stylesheets'] AS $condition => $css) {
        // Allow the theme to override its base themes' styles.
        foreach ($css AS $media => $files) {
          foreach ($files AS $file) {
            $stylesheets[$condition][$media][$file] = $path;
          }
        }
      }
    }
  }
  // Render the stylesheets to link elements.
  $conditional_styles = '';
  if (!empty($stylesheets)) {
    $query_string = '?'. substr(variable_get('css_js_query_string', '0'), 0, 1);
    $base_path = base_path();
    foreach ($stylesheets AS $condition => $css) {
      // Each condition requires its own set of links.
      $output = '';
      foreach ($css AS $media => $files) {
        foreach ($files AS $file => $path) {
          // Don't allow non-existent stylesheets to clutter the logs with 404.
          if (file_exists("./$path/$file")) {
            $output .= "<link type=\"text/css\" rel=\"stylesheet\" media=\"$media\" href=\"$base_path$path/$file$query_string\" />\n";
          }
        }
      }
      if ($output) {
        $conditional_styles .= "<!--[$condition]>\n$output<![endif]-->\n";
      }
    }
  }
  // Save the stylesheets for later retrieval.
  if ($conditional_styles) {
    variable_set('conditional_styles_' . $theme, $conditional_styles);
  }
  else {
    variable_del('conditional_styles_' . $theme);
  }
  // Return nothing.
  return array();
}
