from flask import Flask
from flask_restful import Resource, Api
from flask_restful.reqparse import RequestParser
from flask_pymongo import PyMongo

#Create Flask App
app = Flask(__name__)

#Configure DB settings
app.config['MONGO_DBNAME'] = "air_pollution"
app.config['MONGO_URI'] = "mongodb://127.0.0.1:27017/air_pollution"

#Create a mongo instance
mongo = PyMongo(app)

#Create an api instance
api = Api(app, prefix='/FlaskApp')

#Create JSON Parser
dataParser = RequestParser(bundle_errors=True)


dataParser.add_argument("dev_id")
dataParser.add_argument("payload_fields", type=dict)
dataParser.add_argument("metadata", type=dict)

payloadParser = RequestParser()
payloadParser.add_argument("temp", type=int, location='payload_fields')


payloadParser.add_argument("hum", type=int, location='payload_fields')


payloadParser.add_argument("pm10", type=int, location='payload_fields')


payloadParser.add_argument("pm25", type=int, location='payload_fields')

metadataParser = RequestParser()
metadataParser.add_argument("time", location='metadata')


#Create Resource for data
class airData(Resource):
   def get(self):
        data = mongo.db.air_data
        output = []
        for q in data.find().limit(230):
		str1=str(q['time'])
		date=str1[0:10]
		time=str1[11:19]
		output.append({
			'dev_id' : q['dev_id'],
			'temp' : q['temp'],
			'hum' : q['hum'],
			'pm10' : q['pm10'],
			'pm25' : q['pm25'],
			'time' : time,
			'date' : date
		})
	return output

   def post(self):
        request = dataParser.parse_args()
        payload = payloadParser.parse_args(req=request)
        metaData = metadataParser.parse_args(req=request)
        new_data = {
            	'dev_id' : request['dev_id'],
            	'temp' : payload['temp'],
	        'hum' : payload['hum'],
   	   	'pm10' : payload['pm10'],
	    	'pm25' : payload['pm25'],
		'time' : metaData['time']
        }
        data = mongo.db.air_data
        post_id = data.insert(new_data)
        post_inserted = data.find_one({'_id' : post_id})
        new_data = {
            'dev_id' : post_inserted['dev_id'],
            'temp' : post_inserted['temp'],
            'hum' : post_inserted['hum'],
            'pm10' : post_inserted['pm10'],
	    'pm25' : post_inserted['pm25'],
	    'time' : post_inserted['time']
        }
        return { "success":True, "data":new_data }, 201
   
#Add the resource to the API
api.add_resource(airData, "/air_data")

if __name__ == "__main__":
    app.run()

