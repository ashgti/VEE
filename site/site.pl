#/usr/bin/env perl
use Mojolicious::Lite;
use Mojo::Upload;
use YAML qw(DumpFile Dump LoadFile);

app->log->level("debug");

my $config = LoadFile("config.yaml");

# print Dump($config);

END {
    # Update the configurations
    DumpFile("config.yaml", $config);
}

get '/' => sub {
    my $self = shift;
    
    my $stuff = 'foo bar';
    return $self->render('configuration', stuff => $stuff);
};

get '/run_test' => sub {
    my $self = shift;
    
    return $self->render('run_test');
};

post '/run_test_submit' => sub {
    my $self = shift;
    my $upload = Mojo::Upload->new;
    if ($upload) {
        app->log->debug("got: " . $self->param);
    }
    else {
        app->log->debug("Non");
    }
    
    return $self->render('run_test_submit');
};

get '/run_test_submit' => sub {
    my $self = shift;
    
    return $self->render('test_response');
};

app->start;
