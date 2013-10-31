<?php

$plugin = array(
  'title' => t("SearchAPI Searcher"),
  'description' => t('A SearchAPI Searcher, defaults to first active'),
  'context' => 'panels_search_context_create_searcher',
  'settings form' => 'panels_search_context_searcher_settings_form',
  'settings form validate' => 'panels_search_context_searcher_settings_form_validate',
  'settings form submit' => 'panels_search_context_searcher_settings_form_submit',
  'keyword' => 'searcher',
  'defaults' => array('type' => 'current'),
  'context name' => 'searcher',
);

/**
 */
function panels_search_context_create_searcher($empty, $data = NULL, $conf = FALSE) {
  $context = new ctools_context('string');
  $context->plugin = 'string';

  // Make sure to return a context obj for admin requests. 
  if ($empty || arg(0) == 'admin' || arg(0) == 'panels') {
    return $context;
  }

  $data = current(facetapi_get_active_searchers());

  if (!empty($data)) {
    $context->data     = $data;
    $context->title    = $data;
    return $context;
  }
}

function panels_search_context_searcher_settings_form($conf) {
  $form = array();
  return $form;
}

/**
 * Validate a searcher.
 */
function panels_search_context_searcher_settings_form_validate($form, &$form_values, &$form_state) {
}

function panels_search_context_searcher_settings_form_submit($form, &$form_values, &$form_state) {
}
