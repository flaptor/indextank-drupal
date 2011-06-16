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
  error_log("PREPROCESS: " . print_r($vars,true));

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


$it_api_url = variable_get('indextank_public_apiurl', '');
$it_script = <<< EOD
    $(document).ready(function() { 
       $("#search-block-form").indextank_Ize( "$it_api_url", "drupal");
       var opt = {
         name: "title",
         fields: "title,url,content",
         format: function( item , options ) {

            function hl(text, query){
                text = decodeURIComponent( escape( text ) );
                rx = new RegExp(query,'ig');
                bolds = $.map(text.match(rx) || [], function(i) { return "<span class='highlighted'>"+i+"</span>";});
                regulars = $( $.map(text.split(rx), function(i){ return $("<span></span>").addClass("regular").text(i).get(0);}));

                return regulars.append(function(i, h) {
                    return bolds[i] || ""; 
                });
            };


            var name = item[options.name];
            var highlightedName = hl(name, item.queryTerm);
            
            
            var l =  $("<a></a>").attr("href", item[options.url]);

            // only display images for those documents that have one
            if (item[options.thumbnail]) {
                l.addClass("with-thumbnail");
                l.append( $("<img />")
                            .attr("src", item[options.thumbnail])
                            .css( { "max-width": "50px", "max-height": "50px"} ) ) ;
            }

            l.append( $("<span/>").addClass("name").append(highlightedName) );
            
            // only add description for those documents that have one
            if (item[options.description]) { 
                l.addClass("with-description");
                l.append( $("<span/>").addClass("description").text(item[options.description]));
            }

            return l;
        }
       };
       var fmt = function(item){
           var d = new Date(item.timestamp * 1000);

           return $("<div/>").addClass("node")
                    .append( $("<h2/>")
                                .append( $("<a/>").attr("href", item.url).text(decodeURIComponent( escape( item.title ) ) ) ) )
                    .append( $("<div/>").addClass("meta").addClass("submitted")
                                .text("submitted by " + item.user + " on " + d.format("ddd, mm/dd/yyyy - hh:MM") ))
                    .append( $("<div/>").addClass("content").addClass("clearfix").html(decodeURIComponent( escape( item.snippet_content ) ) ))
                    .append( $("<div/>").addClass("link-wrapper").text("some links"));
       };

       var r = $("#block-system-main").indextank_Renderer({format: fmt});
       var rw = function(q) {
        return "title:(" + q + "*)^5 OR content:(" + q + "*)";
       };
        $("#search-block-form input[type=text]:first").indextank_InstantLinks(opt).indextank_AjaxSearch({listeners: r, rewriteQuery: rw, fields: "title,content,user,timestamp,url", snippets: "content"});
    }); 
EOD;

drupal_add_js($it_script, array('type' => 'inline', 'scope' => 'footer', 'weight' => 5) );

