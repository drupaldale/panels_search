<?php

$plugin = array(
  'title' => t('Panels Search Facet'),
  'content type' => 'panels_search_panels_search_facets_content_type',
  'description' => t('Search Facet that works with Views Search blocks'),
  'defaults' => array(
    'search_id' => 'Auto',
  ),
);

/**
 * Returns the facet for the specified field
 */
function panels_search_panels_search_facets_content_type($subtype_id, $plugin) {
  panels_search_register_facet($subtype_id);
  $info = array();
  $info['render last'] = TRUE;
  return $info;
}

/**
 * Return the facet subtypes. Basically any indexed field. Unlike the normal
 * facetapi blocks, the subtype is not tied to a specific search index and
 * do not need to be enabled via a gui.
 */
function panels_search_panels_search_facets_content_type_content_types() {
  $types = array();
  $indexes = panels_search_facet_indexes();
  foreach ($indexes as $index) {
    foreach ($index->getFields() as $key => $field) {
      $info = panels_search_facet_info($key, $field);
      $types[$key] = $info;
    }
  }
  return $types;
}

function panels_search_facet_info($key, $field) {
  $title = strip_tags($field['description']);
  if ($title == '') {
    $title = $field['name'];
  }

  $info = array(
    'title' => 'Facet: '.$title,
    'category' => 'Panels Search',
  );
  return $info;
}

/**
 * Render the custom content type.
 */
function panels_search_panels_search_facets_content_type_render($subtype, $conf, $panel_args, $context) {
  // Build the content type block.
  // Asume we only have one active searcher.
  $searcher = current(facetapi_get_active_searchers());
  $adapter = facetapi_adapter_load($searcher);

  
  // split this out?
  $adapter->processFacets(); // make a singular version of this?
  $facet = $adapter->getFacet(array('name'=>$subtype));
  $processor = $adapter->getProcessor($subtype);
  $realm = facetapi_realm_load('block');
  // $processor->getBuild() has the base render array.  
  if(!$processor) {
  } 

  if($processor) {
    $content = $facet->build($realm, $processor);
  }

  $block = new stdClass();
  $block->title = $subtype;
  $block->content = $content;
  return $block;
}

/**
 * Returns an edit form for custom type settings.
 */
function panels_search_panels_search_facets_content_type_edit_form($form, &$form_state) {
  $conf = $form_state['conf'];
  $indexes = panels_search_facet_indexes();

  $index_names = array();
  $index_names['auto'] = 'auto';
  foreach($indexes as $key => $index) {
    $index_names[$key] = $key;
  }

  $form['search_id'] = array(
    '#type' => 'select',
    '#title' => t('Index'),
    '#options' => $index_names,
    '#default_value' => $conf['search_id'],
  );
  return $form;
}

/**
 * Submit handler for search form.
 */
function panels_search_panels_search_facets_content_type_edit_form_submit($form, &$form_state) {
  // Copy everything from our defaults.
  foreach (array_keys($form_state['plugin']['defaults']) as $key) {
    $form_state['conf'][$key] = $form_state['values'][$key];
  }
}
