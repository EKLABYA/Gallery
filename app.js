var express = require('express')
var app = express()
var bodyParser = require('body-parser');
var mongoose = require('mongoose')

 
var fs = require('fs');
var path = require('path');
require('dotenv/config');
mongoose.connect(process.env.MONGO_URL,
    { useNewUrlParser: true, useUnifiedTopology: true }, err => {
        console.log('connected')
    })
mongoose.set('strictQuery', false);
app.use(bodyParser.urlencoded({ extended: false }))
app.use(bodyParser.json())
 
// Set EJS as templating engine
app.set('views', path.join(__dirname, 'views'))
app.set("view engine", "ejs");


var multer = require('multer');
 
var storage = multer.diskStorage({
    destination: (req, file, cb) => {
        cb(null, 'uploads')
    },
    filename: (req, file, cb) => {
        cb(null, file.fieldname + '-' + Date.now())
    }
});
 
var upload = multer({ storage: storage });
var imgModel = require('./model');
//login page
app.get('/', (req, res) => {
   res.render('login');
});
//take to the login admin page
app.post('/loginadmin',(req,res)=>{
    res.render("loginadmin")
})

//checking the login 
app.post('/checklogin',function(req,res){
    var mp=req.body.email;
    console.log(mp=="royeklabya@gmail.com");
    console.log(req.body.password=="1234");
    if((mp=="royeklabya@gmail.com") && req.body.password=="1234")
    {
        imgModel.find({}, (err, items) => {
            if (err) {
                console.log(err);
                res.status(500).send('An error occurred', err);
            }
            else {
                res.render('imagesPage', { items: items });
            }
             }
     )}
     else
     {
       res.send("Enter the right credential");
     }
   
  })

app.post('/', (req, res) => {
    imgModel.find({}, (err, items) => {
        if (err) {
            console.log(err);
            res.status(500).send('An error occurred', err);
        }
        else {
            res.render('imagesPage', { items: items });
        }
    });
});


app.post('/client', upload.single('image'), (req, res, next) => {
 
    var obj = {
        name: req.body.name,
        email:req.body.email,
        desc: req.body.desc,
        img: {
            data: fs.readFileSync(path.join(__dirname + '/uploads/' + req.file.filename)),
            contentType: 'image/png'
        }
    }
    imgModel.create(obj, (err, item) => {
        if (err) {
            console.log(err);
        }
        else {
            // item.save();
            imgModel.find({}, (err, items) => {
                if (err) {
                    console.log(err);
                    res.status(500).send('An error occurred', err);
                }
                else {
                    res.render('imagesPage', { items: items });
                }
           
        })}
    });
});

app.post("/delete",(req,res)=>{
    
    imgModel.deleteOne({ email: req.body.email}).then((user)=>{

        if(user.deletedCount === 0)
        {
        
         //res.redirect('/client');
        }
        else{
        //Drone.deleteOne({id:req.body.id} );
        imgModel.find({}, (err, items) => {
            if (err) {
                console.log(err);
                res.status(500).send('An error occurred', err);
            }
            else {
                res.render('imagesPage', { items: items });
            }
        })
  }})
})
app.post('/loginclient',function(req,res){
   
    imgModel.find({email:"99@gmail.com"}, (err, items) => {
        if (err) {
            console.log(err);
            res.status(500).send('An error occurred', err);
        }
        else {
            res.render('imagespageclient', { items: items });
        }
         })
  })
app.post('/loginclientfinal', upload.single('image'), (req, res, next) => {
 
    var obj = {
        name: req.body.name,
        email:req.body.email,
        desc: req.body.desc,
        img: {
            data: fs.readFileSync(path.join(__dirname + '/uploads/' + req.file.filename)),
            contentType: 'image/png'
        }
    }
    imgModel.create(obj, (err, item) => {
        if (err) {
            console.log(err);
        }
        else {
            // item.save();
            imgModel.find({email:req.body.email}, (err, items) => {
                if (err) {
                    console.log(err);
                    res.status(500).send('An error occurred', err);
                }
                else {
                    res.render('imagespageclient', { items: items });
                }
                })
        }
    
    });
})
app.post("/Logout",(req,res)=>{
    res.render("login");
})
var port = process.env.PORT || '3000'
app.listen(port,"0.0.0.0", err => {
    if (err)
        throw err
    console.log('Server listening on port', port)
})