from django.urls import path
from django.conf.urls import url

# Access the views for the api
from api import views

# Defines the app name
app_name = 'api'

urlpatterns = [
    path('', views.index, name='index'),
]


