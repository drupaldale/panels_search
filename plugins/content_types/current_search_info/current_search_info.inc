<?php

$plugin = array(
  'single' => TRUE,
  'title' => t('Panels Search Current Search'),
  'description' => t('Block showing current search with Facet support'),
  'category' => t('Panels Search'),
  'required context' => new ctools_context_required(t('Searcher'), 'string'),
  'render last' => TRUE,
);

/**
 * Render the custom content type.
 */
function panels_search_current_search_info_content_type_render($subtype, $conf, $panel_args, $context) {
  $searcher = current(facetapi_get_active_searchers());
  $adapter = facetapi_adapter_load($searcher);
  $activeItems = $adapter->getAllActiveItems();
  $facets = array();
  foreach($activeItems as $item) {
    $value = $item['value'];
    $alias = $item['field alias'];
    $facet = $adapter->getFacet($alias)->getFacet();
    $type = $facet['field type'];
    $term = taxonomy_term_load($value);
    $facets[] = $term;
  }

  // Build the content type block.
  $block = new stdClass();
  $content = 'hi';
  $block->content = $content;
  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function panels_search_current_search_info_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  return $form;
}

/**
 * Submit handler for search form.
 */
function panels_search_current_search_info_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
?>
