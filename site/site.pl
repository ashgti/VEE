#/usr/bin/env perl
use strict;
use wanrings;
use mro 'c3';
use encoding 'utf8';
use Mojolicious::Lite;

get '/' => sub {
    
}

app->start;
