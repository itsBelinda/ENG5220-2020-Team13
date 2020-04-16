from django.shortcuts import render, redirect, reverse
from django.http import HttpResponse
from django.contrib.auth import authenticate, login, logout
from django.contrib.auth.decorators import login_required

# The main index page for the website.
def index(request):

	# Render and return the index template.
	response = render(request, 'web/index.html')
	return response

# Register a new account.
def user_signup(request):
	response = render(request, 'web/signup.html')
	return response

# For logging into your account.
def user_login(request):
	response = render(request, 'web/login.html')
	return response

# For logging out of the account.
def user_logout(request):
	return HttpResponse('Logout page!')

# For the user account.
def user_account(request):
	response = render(request, 'web/account.html')
	return response