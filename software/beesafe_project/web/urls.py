from django.urls import path
from django.conf.urls import url

# Access the views for the web app.
from web import views

# Enables the use of template and URL tags; eases implementation.
app_name = 'web'

# Parameter for path function is the string to match.
# Empty means that a match will be made when nothing is there.
# Second parameter denotes the view to be called.
# Third parameter, name, is an _optional_ parameter.
# Provides a concenient way to reference the view.
urlpatterns = [
    path('', views.index, name='index'),
    path('signup/', views.user_signup, name='signup'),
    path('login/', views.user_login, name='login'),
    path('logout/', views.user_logout, name='logout'),
    path('account/', views.user_account, name='account'),
]