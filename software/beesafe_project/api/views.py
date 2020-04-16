from django.shortcuts import render, redirect, reverse
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required

# Define the index page i.e. home page for the api app.
def index(request):
	return HttpResponse('Api, Hello World!')

