<?php
/**
* Override or insert PHPTemplate variables into the search_theme_form template.
*
* @param $vars
*   A sequential array of variables to pass to the theme template.
* @param $hook
*   The name of the theme function being called (not used in this case.)
*/
function indextank_preprocess_search_theme_form(&$vars, $hook) {

  // Remove the "Search this site" label from the form.
  $vars['form']['search_theme_form']['#title'] = t('Search this site');
  
  // Set a default value for text inside the search box field.
  //$vars['form']['search_theme_form']['#'] = t('Powered by IndexTank');
  
  // Add a custom class and placeholder text to the search box.
  // $vars['form']['search_theme_form']['#attributes'] = array('placeholder' => 'Powered by IndexTank', 'style' => 'min-width:125px');
  
  // Change the text on the submit button
  //$vars['form']['submit']['#value'] = t('Go');
 
  // Rebuild the rendered version (search form only, rest remains unchanged)
  unset($vars['form']['search_theme_form']['#printed']);
  $vars['search']['search_theme_form'] = drupal_render($vars['form']['search_theme_form']);
  
  //die($vars['search']['search_theme_form']);

  $vars['form']['submit']['#type'] = 'button';
  //$vars['form']['submit']['#src'] = path_to_theme() . '/images/search.jpg';
    
  // Rebuild the rendered version (submit button, rest remains unchanged)
  unset($vars['form']['submit']['#printed']);
  $vars['search']['submit'] = drupal_render($vars['form']['submit']);

  // Rebuild the rendered version (submit button, rest remains unchanged)
  // unset($vars['form']['script']['#printed']);
  $vars['search']['script'] = '<script>'.
    '$(document).ready(function(){'.
      '$("#search-theme-form").indextank_Ize("' . variable_get('indextank_public_apiurl', '') . '", "drupal");'.
      'var opt = {name:"title", url:"url", fields:"title,url,content"};'.
      '$("#edit-search-theme-form-1").indextank_InstantLinks(opt);'.
    '});'.
  '</script>';  
  // Collect all form elements to make it easier to print the whole form.
  $vars['search_form'] = implode($vars['search']);
}

?>

